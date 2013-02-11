#include "cs_hash.h"

void
data_free(void *data) {
  free(data);
}

void PRINT_HASH(cs_hash_t *hash_table){
  int count = 0;
  cs_bucket_t *p = hash_table->g_head;
  if (!p) {printf("current count %d\n", hash_table->elem_count);return;}
  do{
    count++;
    printf("%s", p->data);
    p = p->g_next;
  }while(p != hash_table->g_head);
  printf("recusvice cout %d\n", count);
  printf("current hash size %d\n", hash_table->elem_count);
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

  fclose(fp);

  fp = fopen("/usr/share/dict/words", "r");
  while(!feof(fp)){
    result = fgets(buf, 256, fp);
    char* value = cs_hash_search(hash_table, buf, strlen(buf)+1);
    if (EOF != value) {
       cs_hash_delete(hash_table, buf, strlen(buf)+1);
    } else{
      printf("error search %s\n", buf);
      return -1;
    }
  } 
  PRINT_HASH(hash_table);

  fclose(fp);
  cs_hash_free(hash_table);
  return 0;
}
