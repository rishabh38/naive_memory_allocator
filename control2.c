#include "libmalloc.h"
#include "log.h"

#include <stdio.h>


void load(int rounds, int percent, size_t size) {
    void *ptrs[percent];

    for (int i = 0; i < rounds; i++) {
        for (int j = 0; j < percent; j++) {
            ptrs[j] = esmalloc(size);
        }
        for (int j = 0; j < 100 - percent; j++) {
            esfree(ptrs[j]);
        }
    }

    struct heapStatus heapstatus = getHeapUsageStatus();
    heaplog(heapstatus);
}


int main() {
#define FILENAME "heaplog"
    if (initlog(FILENAME) < 0) {
        fprintf(stderr, "unable to initialize log file\n");
        return -1;
    }
#define ROUNDS 1000
#define SIZE 1000
    load(ROUNDS, 80, SIZE);
}

    
