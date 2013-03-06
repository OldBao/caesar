#ifndef CS_CONN_H_
#define CS_CONN_H_

#include "caesar.h"

typedef struct cs_conn_s cs_conn_t;
typedef struct cs_conn_handler_s cs_conn_handler_t;

struct cs_conn_s {
  int fd;
  struct sockaddr_info *remote_addr;
    
  int blocking:1;

  cs_bool (*cs_read_handler)(cs_conn_t *);
  cs_bool (*cs_write_handler)(cs_conn_t *);
};

cs_conn_t* cs_conn_new();
void cs_conn_shutdown(cs_conn_t *conn);
void cs_conn_release(cs_conn_t *conn);
#endif
