#ifndef LIBMALLOC_H
#define LIBMALLOC_H

#include <stddef.h>

void *esmalloc(size_t size);
void esfree(void *addr);

#endif
