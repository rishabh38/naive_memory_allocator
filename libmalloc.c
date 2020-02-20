#include "libmalloc.h"

#include <unistd.h>

#define GAP 5


static void *nextallocaddr;
static int initstatus = 0;

static void init() {
    nextallocaddr = sbrk(0);
    initstatus = 1;
}

void *esmalloc(size_t size) {
    if (initstatus == 0) {
        init();
    }

    if (nextallocaddr+size >= sbrk(0)) {
        sbrk(size+GAP);
    }
    
    void *dupnaa = nextallocaddr;
    nextallocaddr += size;
    return dupnaa;
}

void esfree(void *addr) {}
