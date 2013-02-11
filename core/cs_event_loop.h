#ifndef CS_LOOP_H_
#define CS_LOOP_H_

#include "caesar.h"

typedef struct cs_event_loop_s cs_event_loop_t;
typedef struct cs_event_s      cs_event_t;


bool delete_event(cs_event_t *event);
bool add_event(cs_event_t *event);
void start_loop(cs_event_loop_t *event_loop); 
#endif
