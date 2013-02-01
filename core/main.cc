#include "cs_hash.h"

void
data_free(void *data) {
  free(data);
}

int
main() {
  FILE *fp;
  char buf[256];
  fp = fopen("/usr/share/dict/words", "r");
  if (!fp)
    printf("error open\n");
  
  cs_hash_t *hash_table = cs_hash_new_ex(CS_HASH_USE_DEFAULT_SIZE, 
					 data_free, NULL);
  while(!feof(fp)){
    fgets(buf,256, fp);
    cs_hash_insert(hash_table, buf, strlen(buf)+1, buf, strlen(buf)+1);
  }
  
  printf("current hash size %d\n", hash_table->elem_count);
  //  fp = fopen("/usr/share/dict/words", "r");
  //while(!feof(fp)){
  //  fgets(buf, 256, fp);
  //  char* value = cs_hash_search(hash_table, buf, strlen(buf));
  //  if (value) {
  //    printf("search hits %s\n", value);
  //  } else{
  //    printf("error search %s\n", value);
  //    return -1;
  //  }
  //}
 
  cs_hash_free(hash_table);
  return 0;
}
