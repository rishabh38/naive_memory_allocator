#ifndef LIBMALLOC_H
#define LIBMALLOC_H

#include <stddef.h>

struct heapStatus {
    size_t totalspace;
    size_t unusedspace;
    size_t usedspace;
};

void *esmalloc(size_t size);
void esfree(void *addr);
struct heapStatus getHeapUsageStatus();

#endif
