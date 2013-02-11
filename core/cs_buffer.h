#ifndef CS_BUFFER_H_
#define CS_BUFFER_H_

#include "caesar.h"

typedef cs_buffer_s cs_buffer_t;

struct cs_buffer_s {
  char *start;
  char *current;

  int   len;
  
  cs_buffer_t *next;
};

#define __CS_DEFAULT_BUFFER_SIZE 4096

#define cs_buffer_new cs_buffer_new_ex(NULL, __CS_DEFAULT_BUFFER_SIZE)
#define cs_buffer_append(prev) cs_buffer_new_ex(prev, __CS_DEFAULT_BUFFER_SIZE)
cs_buffer_t* cs_buffer_new_ex(cs_buffer_t *prev, int advised_len);

#define cs_buffer_free_one(buffer) cs_buffer_free(buffer, buffer)
#define cs_buffer_free_all(buffer) cs_buffer_free(buffer, NULL)

cs_buffer_t* cs_buffer_free(cs_buffer_t* head, cs_buffer_t* tail);

cs_buffer_t* 
#endif
