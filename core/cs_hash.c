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


#define RELEASE_IN_LOCAL_LIST(slot, bucket) {     \
  if (bucket->prev == bucket) {  		  \
      slot = NULL;				  \
  } else {					  \
      bucket->prev->next = bucket->next;	  \
      bucket->next->prev = bucket->prev;	  \
      if (slot == bucket) {                       \
         slot = bucket->next;                     \
      }                                           \
   }						  \
}

#define RELEASE_IN_GLOBAL_LIST(ht, bucket)        \
  if (bucket->g_prev == bucket) {	          \
      ht->g_head = NULL;			  \
  } else {					  \
      bucket->g_prev->g_next = bucket->g_next;	  \
      bucket->g_next->g_prev = bucket->g_prev;	  \
      if (bucket == ht->g_head){		  \
         ht->g_head = bucket->g_next;             \
      }                                           \
  }						  \
}

#define RESIZE_IF_FULL(hash) {                                \
  if (hash->table_size < hash->elem_count){                   \
     cs_hash_resize(hash, hash->factor * hash->table_size);   \
  }							      \
}


#ifdef NDEBUG
#define CHECK_HASH(hash)
#else
#define CHECK_HASH(hash) cs_hash_check(hash)
#endif

static cs_bool cs_hash_check(cs_hash_t *hash);

cs_hash_t*
cs_hash_new_ex(uint32_t size, 
	       cs_hash_free_func_t free_func, 
	       cs_hash_cmp_func_t  cmp_func)
{
  cs_hash_t *new_hash = (cs_hash_t *)cs_malloc(sizeof(cs_hash_t));
  
  if (NULL == new_hash){
    CS_FATAL("create hash table error");
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

  cs_hash_key_t hash_key = cs_hash_func(key, key_len);
  uint32_t index = hash_key % CS_HASH_MOD(hash);
  cs_bucket_t *p;

  p = hash->buckets[index];
  if (p){
    do{
      if (p->key_len == key_len && p->hash_key == hash_key){
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

  if (!p){
    CS_WARNING("can't find element\n");
    return FALSE;
  }
  do {
    if (p->key_len == key_len) {
      if (0 == hash->cmp_func(p->key, key, key_len)) {
	RELEASE_IN_LOCAL_LIST(hash->buckets[index], p);
	RELEASE_IN_GLOBAL_LIST(hash, p);
	if (hash->free_func) {
	  hash->free_func(p->data);
	}
	hash->elem_count--;
	CS_DEBUG("deleted %s", key);
	return TRUE;
      }
      p = p->next;
    }while(p != hash->buckets[index]);
  
    CS_WARNING("can't find element\n");
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

cs_bool
cs_hash_resize(cs_hash_t* hash, uint32_t new_size) {
  assert(new_size > hash->table_size);

  cs_bucket_t   **new_table = (cs_bucket_t **) cs_malloc(sizeof(cs_bucket_t *) * new_size);
  memset(new_table, 0, sizeof(cs_bucket_t *) * new_size);
  cs_bucket_t   *p = hash->g_head;
  cs_hash_key_t key;
  uint32_t      index;

  if (!new_table)
    return FALSE;
  
  if (!p)
    return TRUE;

  hash->table_size = new_size;
  do {
    index = p->hash_key % CS_HASH_MOD(hash);
    if (0 == strcmp("études\n", p->data)) {
      printf("rehash études's to %d\n", index);
    }
    // because we rehash the table, so when we don't need check exists of current value
    INSERT_TO_LOCAL_LIST(new_table[index], p);
    p= p->g_next;
  } while (p != hash->g_head);

  cs_free(hash->buckets);
  hash->buckets = new_table;
  CS_DEBUG("resize hash table from to %d", new_size);
  CHECK_HASH(hash);
}


static cs_bool
cs_hash_check(cs_hash_t* hash) {
  cs_bucket_t *p;
  uint32_t    i;
  
  for (i = 0; i < hash->table_size; i++) {
    if (hash->buckets[i]){
      p = hash->buckets[i];
      do {
	assert(p->data_len > 0);
	assert(p->hash_key % CS_HASH_MOD(hash) == i);
	
	p = p->next;
      }while(p != hash->buckets[i]);
    }
  }
}
