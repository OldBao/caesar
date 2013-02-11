#ifndef CS_LOG_H_
#define CS_LOG_H_

#include <stdio.h>

#define CS_FATAL(msg,   arg...)   printf("fatal: " msg "\n", ##arg)
#define CS_WARNING(msg, arg...)   printf("warning" msg "\n", ##arg)
#define CS_NOTICE(msg,  arg...)   printf("notice:" msg "\n", ##arg)
#define CS_DEBUG(msg,   arg...)   printf("debug:"  msg "\n", ##arg)
#define CS_TRACE(msg,   arg...)   printf("trace:"  msg "\n", ##arg)
#endif
