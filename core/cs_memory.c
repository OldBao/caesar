#include "caesar.h"
#include "cs_memory.h"

void*
cs_malloc(size_t len) {
  return malloc(len);
}

void
cs_free(void* addr) {
  free(addr);
}

void*
cs_realloc(void* addr, size_t len) {
  return realloc(addr, len);
}
