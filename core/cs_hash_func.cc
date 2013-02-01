#include <cs_hash.h>

cs_hash_key_t
cs_hash_func(void *key, uint32_t key_len) {
  uint64_t result = 0;
  int i;
  for(i = 0; i < key_len; i++) {
    result = 33 * result + ((uint8_t *)key)[i];
  }
  return result;
}
