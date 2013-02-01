/**
 * @file   cs_hash.h
 * @author zhanggx <zhangguanxing01@baidu.com>
 * @date   Fri Feb  1 12:17:19 2013
 * 
 * @brief hash table  
 * 
 * 
 */

#ifndef CS_HASH_H_
#define CS_HASH_H_

#include "caesar.h"

typedef struct   cs_hash_s   cs_hash_t;
typedef struct   cs_bucket_s cs_bucket_t;
typedef uint32_t cs_hash_key_t;

typedef void (*cs_hash_free_func_t)(void *data);
typedef int  (*cs_hash_cmp_func_t)(const void *data1, const void* data2, size_t n);

struct cs_hash_s {
  cs_bucket_t      **buckets;
  uint32_t         elem_count;
  uint32_t         table_size;
  float            factor; //increasing  factor

  cs_bucket_t      *g_head;

  //function handlers
  cs_hash_cmp_func_t  cmp_func;
  cs_hash_free_func_t free_func;
};

struct cs_bucket_s {
  cs_hash_key_t hash_key;
 
  void          *key;
  uint32_t      key_len;
  void          *data;
  uint32_t      data_len;
 
  cs_bucket_t   *next;
  cs_bucket_t   *prev;
  cs_bucket_t   *g_prev;
  cs_bucket_t   *g_next;
};

cs_hash_key_t cs_hash_func(void* key, uint32_t key_len);
#define CS_HASH_USE_DEFAULT_SIZE -1

//ctor
#define cs_hash_new() cs_hash_new_ex(CS_HASH_USE_DEFAULT_SIZE, NULL, NULL)
#define cs_hash_new_with_free(func) cs_hash_new_ex(CS_HASH_USE_DEFAULT_SIZE, func, NULL)
#define cs_hash_new_with_cmp(func) cs_hash_new_ex(CS_HASH_USE_DEFAULT_SIZE, NULL, func)
#define cs_hash_new_with_size(size) cs_hash_new_ex(size, NULL, NULL)

/** 
 * 
 * 
 * @param advise_len advised table length, if set CS_HASH_SIZE_USE_DEFAULT
                     it will use default size
 * @param free_func 
 * 
 * @return new hash table
 */
cs_hash_t* cs_hash_new_ex(uint32_t advise_len, 
			  cs_hash_free_func_t free_func,
			  cs_hash_cmp_func_t  cmp_func);

/** 
 * 
 * 
 * @param hash 
 * @param key 
 * @param key_len 
 * @param data 
 * @param data_len 
 * 
 * @return 
 */
cs_bool cs_hash_insert(cs_hash_t *hash, void *key, uint32_t key_len, void *data, uint32_t data_len);

/** 
 * 
 * 
 * @param hash 
 * @param key 
 * @param key_len 
 * 
 * @return 
 */
cs_bool cs_hash_delete(cs_hash_t *hash, void *key, uint32_t key_len);

/** 
 * 
 * 
 * @param key 
 * @param key_len 
 * 
 * @return 
 */
void* cs_hash_search(cs_hash_t *hash, void* key, uint32_t key_len);

/** 
 * 
 * 
 * @param hash hash table 
 */
void cs_hash_free(cs_hash_t *hash);

#endif
