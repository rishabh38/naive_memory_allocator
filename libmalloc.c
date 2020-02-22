#include "libmalloc.h"

#include <stdbool.h>
#include <unistd.h>

#define GAP 5
#define HEAPSPACECHECKON


static void *nextallocaddr;
static int initstatus = 0;

static void init() {
    nextallocaddr = sbrk(0);
    initstatus = 1;
}

#ifdef HEAPSPACECHECKON
struct blockinfo {
    bool inuse;
    void *address;
    size_t length;
};

#define MAXHEAPBLOCKS 1000000
static struct blockinfo heapBlocks[MAXHEAPBLOCKS];
static size_t blocksIndex;
#endif

void *esmalloc(size_t size) {
    if (initstatus == 0) {
        init();
    }

    if (nextallocaddr+size >= sbrk(0)) {
        sbrk(size+GAP);
    }
    
#ifdef HEAPSPACECHECKON
    heapBlocks[blocksIndex].inuse = 1;
    heapBlocks[blocksIndex].address = nextallocaddr;
    heapBlocks[blocksIndex++].length = size;
#endif

    void *dupnaa = nextallocaddr;
    nextallocaddr += size;
    return dupnaa;
}

void esfree(void *addr) {
#ifdef HEAPSPACECHECKON
    for (size_t i = 0 ; i < blocksIndex; i++) {
        if (heapBlocks[i].address == addr) {
            heapBlocks[i].inuse = 0;
        }
    }
#endif
}

#ifdef HEAPSPACECHECKON
/* finds the total space present in the heap,
 * used by the heap and left idle by the heap,
 * returns it in the form of heapStatus structure
 */
struct heapStatus getHeapUsageStatus() {
    struct heapStatus heapstatus = {0, 0, 0};
    
    for (size_t i = 0; i < blocksIndex; i++) {
        heapstatus.totalspace += heapBlocks[i].length;
        if (heapBlocks[i].inuse) {
            heapstatus.usedspace += heapBlocks[i].length;
        }
        else {
            heapstatus.unusedspace += heapBlocks[i].length;
        }
    }

    return heapstatus;
}
#endif
