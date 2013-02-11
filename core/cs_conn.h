#ifndef CS_CONN_H_
#define CS_CONN_H_

#include "caesar.h"

typedef struct cs_conn_s cs_conn_t;

struct cs_conn_s {
  int sockfd;
  struct sockaddr_info *remote_addr;

  int blocking:1;

  cs_buffer_t *recv_buffer;
  cs_buffer_t *send_buffer;
};

cs_conn_t* conn_new();
cs_conn_t* conn_shutdown();
cs_conn_t* conn_release();
cs_conn_t* conn_send(char *packet, size_t packet_len);
#endif
