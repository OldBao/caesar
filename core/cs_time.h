-/**
 * @file   cs_time.h
 * @author Zhang Guanxing <zhangguanxing01@baidu.com>
 * @date   Mon Feb 18 14:05:24 2013
 * 
 * @brief  
 * 
 * 
 */

#ifndef CS_TIME_H_
#define CS_TIME_H_

typedef struct cs_time_s cs_time_t;
typedef struct cs_timer_s cs_timer_t;

extern long cs_global_msec;
 
struct cs_time_s {
  long tv_sec;
  long tv_usec;  
};

struct cs_timer_s {
  cs_rbtree_t timer_tree;
};

cs_timer_t* cs_timer_create();

long cs_timer_next(cs_timer_t* timer);
void cs_timer_global_update();
void cs_timer_free();

#endif
