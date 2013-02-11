#ifndef CS_LISTENER_H_
#define CS_LISTENER_H_

#include "caesar.h"

typedef struct cs_listener_s cs_listener_t;

enum cs_listener_state{INITIAL, BIND, LISTEN, ERROR};

struct cs_listener_s {
  int fd;

  int backlog;
  uint16_t port;
  cs_listener_state state;
  cs_bool deferred_accpet;
};

cs_listener_t* cs_listener_new();
cs_listener_t* cs_listen(cs_listener_t *listener);
cs_listener_t* cs_accept_conn(cs_listener_t *listener);
cs_listener_t* cs_listener_free(cs_listener_t* listener);

#endif
