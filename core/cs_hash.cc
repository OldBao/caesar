#include "cs_hash.h"
#include "cs_memory.h"

#define DEFAULT_TABLE_SIZE (1 << 10) //1kb 
#define MINIMUM_TABLE_SIZE (1 << 5)
#define DEFAULT_INCREASING_FACTOR 1.5
#define CS_HASH_MOD(ht) (ht->table_size - 1)
#define INSERT_TO_LOCAL_LIST(slot, bucket){       \
  if (slot == NULL) {                             \
    slot = bucket;                                \
    bucket->next = bucket;                        \
    bucket->prev = bucket;                        \
  } else{					  \
    slot->prev->next = bucket;                    \
    bucket->prev     = slot->prev;                \
    slot->prev       = bucket;                    \
    bucket->next     = slot;                      \
  }                                               \
}

#define INSERT_TO_GLOBAL_LIST(ht, bucket){        \
  if (ht->g_head == NULL) {			  \
     ht->g_head = bucket;			  \
     bucket->g_prev = bucket;			  \
     bucket->g_next = bucket;			  \
  } else {					  \
    ht->g_head->g_prev->g_next = bucket;	  \
    bucket->g_prev        = ht->g_head->g_prev;	  \
    ht->g_head->g_prev    = bucket;	          \
    bucket->g_next        = ht->g_head;	          \
  }						  \
}

#define RELEASE_IN_LOCAL_LIST(slot, bucket)
#define RELEASE_IN_GLOBAL_LIST(ht, bucket)
#define RESIZE_IF_FULL(hash)

cs_hash_t*
cs_hash_new_ex(uint32_t size, 
	       cs_hash_free_func_t free_func, 
	       cs_hash_cmp_func_t  cmp_func)
{
  cs_hash_t *new_hash = (cs_hash_t *)cs_malloc(sizeof(cs_hash_t));
  
  if (NULL == new_hash){
    FATAL("create hash table error");
    return NULL;
  }
  
   if (size == CS_HASH_USE_DEFAULT_SIZE){
    new_hash->table_size = DEFAULT_TABLE_SIZE;
   } else if (size < MINIMUM_TABLE_SIZE){
    new_hash->table_size = MINIMUM_TABLE_SIZE;
   } else {
    new_hash->table_size = size;
   }
   
   new_hash->buckets    = (cs_bucket_t **)cs_malloc(sizeof(cs_bucket_t *) * new_hash->table_size);
   new_hash->elem_count = 0;
   new_hash->factor     = DEFAULT_INCREASING_FACTOR;
   new_hash->g_head     = NULL;
   
   if (cmp_func)
     new_hash->cmp_func   = cmp_func;
   else
     new_hash->cmp_func = memcmp;

   if (free_func)
     new_hash->free_func  = free_func;
   else
     new_hash->free_func = cs_free;

   memset(new_hash->buckets, 0, sizeof(cs_bucket_t *) * new_hash->table_size);
   
   return new_hash;
}

cs_bool
cs_hash_insert(cs_hash_t *hash, void *key, uint32_t key_len, void *data, uint32_t data_len)
{
  assert(key_len > 0);
  assert(data_len > 0);

  uint32_t hash_key = cs_hash_func(key, key_len);
  uint32_t index = hash_key % CS_HASH_MOD(hash);
  cs_bucket_t *p;

  p = hash->buckets[index];
  if (p){
    do{
      if (p->key_len == key_len){
	  if (0 == hash->cmp_func(p->key, key, key_len)) {
	    //update and return
	    if (hash->free_func){
	      hash->free_func(p->data);
	    }
	    p->data     = cs_malloc(data_len);
	    if (!p->data)
	      return FALSE;
	    p->data_len = data_len;
	    memcpy(p->data, data, data_len);
	    return TRUE;
	  }
      }
      p = p->next;
    }while(p != hash->buckets[index]);
  }

  //add new element
  cs_bucket_t *new_bucket = (cs_bucket_t *) malloc (sizeof(cs_bucket_t));
  new_bucket->hash_key = hash_key;
  new_bucket->key      = cs_malloc(key_len);
  memcpy(new_bucket->key, key, key_len);
  new_bucket->data     = cs_malloc(data_len);
  memcpy(new_bucket->data, data, data_len);
  new_bucket->key_len  = key_len;
  new_bucket->data_len = data_len;
  
  INSERT_TO_LOCAL_LIST(hash->buckets[index], new_bucket);
  INSERT_TO_GLOBAL_LIST(hash, new_bucket);

  hash->elem_count++;

  RESIZE_IF_FULL(hash);
  return TRUE;
}

cs_bool
cs_hash_delete(cs_hash_t *hash, void *key, uint32_t key_len) {
  cs_bucket_t   *p;
  cs_hash_key_t hash_key;
  uint32_t      index;

  hash_key = cs_hash_func(key, key_len);
  index    = hash_key % CS_HASH_MOD(hash);
  p        = hash->buckets[index];

  if (!p)
    return FALSE;
  
  do {
    if (p->key_len == key_len) {
      if (0 == hash->cmp_func(p->key, key, key_len)) {
	RELEASE_IN_LOCAL_LIST(hash->buckets[index], p);
	RELEASE_IN_GLOBAL_LIST(hash, p);
	if (hash->free_func) {
	  hash->free_func(p->data);
	}
	return TRUE;
      }
    }
    p = p->next;
  }while(p != hash->buckets[index]);
  return FALSE;
}

void *
cs_hash_search(cs_hash_t *hash, void *key, uint32_t key_len) {
  cs_hash_key_t hash_key = cs_hash_func(key, key_len);
  uint32_t      index    = hash_key % CS_HASH_MOD(hash);
  cs_bucket_t   *p       = hash->buckets[index]; 

  if (!p) {
    return NULL;
  }
  
  do {
    if (p->key_len == key_len) {
      if (0 == hash->cmp_func(p->key, key, key_len)) {
	return p->data;
      }
    }
    p = p->next;
  }while (p != hash->buckets[index]);
  
  return NULL;
}

void
cs_hash_free(cs_hash_t* hash){
  cs_bucket_t *p = hash->g_head;
  if (p) {
     do {
      if (hash->free_func) {
	hash->free_func(p->data);
      }
      cs_free(p->key);
      p = p->next;
      cs_free(p->prev);
     }while(p != hash->g_head);
  }
  cs_free(hash->buckets);
  cs_free(hash);
}
