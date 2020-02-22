#ifndef LOG_H
#define LOG_H


#include "libmalloc.h"

#define MAXFILENAMESIZE 255

int initlog(char filename[MAXFILENAMESIZE]);
int heaplog(struct heapStatus heapstatus);

#endif
