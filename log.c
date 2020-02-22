#include "log.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static int init;
static long counter;
static FILE *logstream = NULL;

/* function: initlog(char pathname[MAXFILENAMESIZE]
 * sets up the log file stream by opening the log file
 * input:
 * pathname: character array
 * on error returns -1, else returns 1
 */
int initlog(char pathname[MAXFILENAMESIZE]) {
    if ((logstream  = fopen(pathname, "a")) == NULL) {
        fprintf(stderr, "log error: %s\n", strerror(errno));
        return -1;
    }

    fprintf(logstream, "\n\ninitialized logging\n"\
            "time: %ld\n\n", time(NULL));

    init = 1;
    return 1;
}

/* heaplog(heapStatus heapstatus):
 *  logs down the heap data to the logging file
 */
int heaplog(struct heapStatus heapstatus) {

    if (init != 1) {
        fprintf(stderr, "log isn't initialized succesfully yet\n");
        return -1;
    }

    fprintf(logstream, "log no. %lu:\n"\
            "total space: %lu\n"\
            "used space: %lu\n"\
            "unused space: %lu\n", 
            counter++, 
            heapstatus.totalspace,
            heapstatus.usedspace,
            heapstatus.unusedspace
            );

    fprintf(stderr, "log no. %lu:\n"\
            "total space: %lu\n"\
            "used space: %lu\n"\
            "unused space: %lu\n"\
            "waste: %f\n", 
            counter++, 
            heapstatus.totalspace,
            heapstatus.usedspace,
            heapstatus.unusedspace,
            ((float)heapstatus.unusedspace*100)/heapstatus.totalspace
            );
    
    return 1;
}
