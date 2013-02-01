#ifndef CS_LOG_H_
#define CS_LOG_H_

#include <stdio.h>

#define FATAL(msg)   printf("fatal: %s\n", msg)
#define WARNING(msg) printf("warning: %s\n", msg)
#define NOTICE(msg)  printf("notice: %s\n", msg)

#endif
