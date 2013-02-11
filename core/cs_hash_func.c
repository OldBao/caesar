/**
 * @file   cs_hash_func.c
 * @author zhanggx <zhanggx@zhanggx-ThinkPad-X230>
 * @date   Fri Feb  1 20:56:06 2013
 * 
 * @brief  
 * 
 * 
 */
#include <cs_hash.h>

/** 
 * 
 * 
 * @param key 
 * @param key_len 
 * 
 * @return 
 */
cs_hash_key_t
cs_hash_func(void *key, uint32_t key_len) {
  uint64_t result = 0;
  int i;
  for(i = 0; i < key_len; i++) {
    result = 33 * result + ((uint8_t *)key)[i];
  }
  return result;
}
