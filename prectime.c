#include "prectime.h"

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>

prectime getprectime() {
    struct timeval tv;
    if (gettimeofday(&tv, NULL) == -1) {
        fprintf(stderr, "getprectime error: %s\n", strerror(errno));
        return -1;
    }
    prectime time= tv.tv_sec*1000000 + tv.tv_usec;
    return time;
}
