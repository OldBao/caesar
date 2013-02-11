#ifndef CS_MEMORY_H_
#define CS_MEMORY_H_

void* cs_malloc(size_t len);
void  cs_free(void* addr);
void* cs_realloc(void* addr, size_t len);
#endif
