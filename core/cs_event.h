/**
 * @file   cs_event.h
 * @author Zhang Guanxing <zhangguanxing01@baidu.com>
 * @date   Mon Feb 18 12:43:16 2013
 * 
 * @brief  
 * 
 * 
 */

#ifndef CS_EVENT_H_
#define CS_EVENT_H_

#include "caesar.h"

struct cs_event_op_s{
  cs_bool (*init)(cs_event_cycle_t *);
  cs_bool (*add_event)(cs_event_cycle_t *, cs_event_t *, int);
  cs_bool (*mod_event)(cs_event_cycle_t *, cs_event_t *, int);
  cs_bool (*dispatch_once)(cs_event_cycle_t*, long);
  cs_bool (*destroy)(cs_event_cycle_t*);
};

struct cs_event_cycle_s {
  cs_event_op_t    event_ops;
  
  //use for timeout event record
  cs_timer_t      *timer;

  //use for io event
  cs_queue_t     **event_queue;

  cs_listener_t   *listener;
};

#define CS_EVENT_ADD 0x01
#define CS_EVENT_MOD 0x02
#define CS_EVENT_DEL 0x04

enum cs_event_type {CS_LISTEN, CS_NET_IO};
struct cs_event_s {
  cs_event_type type;
  union {
	cs_event_listen  listener;
	cs_event_io      io;
  }e;
};
#endif
