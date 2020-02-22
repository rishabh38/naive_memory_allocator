#include "libmalloc.h"

#include "prectime.h"
#include "timemalloc.h"

#include <inttypes.h>
#include <stdio.h>
#include <string.h>


#define SIZE 10000
#define REQUESTS 1000
#define STAT_MAXRECORDCOUNT 10000

/*
struct addrrange{
    void *startaddr;
    void *endaddr;
};

struct addrrange heavyload(size_t length, size_t size) {
    void *ptr[10];
    struct addrrange range;
    range.startaddr = esmalloc(size);
    for (unsigned i = 0;i  < length; i++) {
        for (unsigned j = 0; j < 10; j++) 
            ptr[j] = esmalloc(size);
        
        esfree(ptr[5]);
    }

    range.endaddr = ptr[9];
}
*/


/*
int main() {
    char message[] = "Caravan";
    char *str = (char *)esmalloc(sizeof(char)*SIZE);
    
    
    printf("str: %p\n", str);
    printf("assigning data to str\n");
    strncpy (str, message, strlen(message));
    
    printf("assigned data: %s\n", str);
    printf("assigned data: ");
    for (int i = 0; i < SIZE; i++) {
        printf("%c", str[i]);
    }
    printf("\n");
    esfree(str);
}
*/


struct report {
    void *startaddr;
    void *endaddr;
    prectime total_time;
    size_t met_len; // stores total length of malloc_elpasedtimes
    size_t fet_len; // stores total length of free_elapsedtimes
    prectime malloc_elapsedtimes[STAT_MAXRECORDCOUNT];  
    prectime free_elapsedtimes[STAT_MAXRECORDCOUNT];
};

/*
struct report load(size_t duration, size_t size, int total, int separation) {
    void *ptrs[10];
    
    prectime starttime = getprectime();
    void *startaddr = esmalloc(size);

    for (int i = 0; i < duration; i++) {
        for (int j = 0; j < separation; j++) {
            ptrs[j] = esmalloc(size);
        }
        for (int j = 0; j < total-separation; j++) {
            esfree(ptrs[j]);
        }
    }
    
    prectime endtime = getprectime();
    void *endaddr = ptrs[9];

    struct report data;
    data.startaddr = startaddr;
    data.endaddr = endaddr;
    data.total_time = endtime - starttime;

    return data;
}
*/

struct report setupreport (
        void *startaddr,
        void *endaddr,
        prectime total_time,
        const prectime *malloc_elapsedtimes,
        const prectime *free_elapsedtimes,
        size_t met_len,
        size_t fet_len
        ) {
    
    struct report data;
    data.startaddr = startaddr;
    data.endaddr = endaddr;
    data.total_time = total_time;

    for (unsigned long i = 0; i < met_len; i++) {
        if (i < fet_len) data.free_elapsedtimes[i] = free_elapsedtimes[i];
        data.malloc_elapsedtimes[i] = malloc_elapsedtimes[i];
    }

    data.met_len = met_len;
    data.fet_len = fet_len;

    return data;
}

struct report load(size_t duration, size_t size, int total, int separation) {
    void *ptrs[10];
    size_t met_len = duration*separation;
    size_t fet_len = duration*(total-separation);
    prectime malloc_elapsedtimes[met_len]; int x =0;
    prectime free_elapsedtimes[fet_len]; int y = 0;
    
    prectime starttime = getprectime();
//    fprintf(stderr,"running initial mallocation\n");   //<------------------------------------------------------
    void *startaddr = esmalloc(size);
//    fprintf(stderr, "crosssed initial mallocation\n");   //<------------------------------------------------------
    for (int i = 0; i < duration; i++) {
        for (int j = 0; j < separation; j++) {
            struct mt_report malloctime = timemalloc(size);
            malloc_elapsedtimes[x++] = malloctime.mt_time;
            ptrs[j] = malloctime.addr;
        }
        
        for (int j = 0; j < total - separation; j++) {
//            fprintf(stderr, "beg");//<------------------------------------------------------
            free_elapsedtimes[y++] = timefree(ptrs[j]);
//            fprintf(stderr, "-start\n");//<------------------------------------------------------
        }
    }
    
    prectime endtime = getprectime();
    void *endaddr = ptrs[9];

    return setupreport(startaddr, endaddr, endtime-starttime, 
            malloc_elapsedtimes, free_elapsedtimes, met_len, fet_len);
}




struct report pureload(size_t duration, size_t size) {
    return load(duration, size, 10, 10);
}

struct report heavyload(size_t duration, size_t size) {
    return load(duration, size, 10, 9);
}

struct report mediumload(size_t duration, size_t size) {
    return load(duration, size, 12, 8);
}

struct report lightload(size_t duration, size_t size) {
    return load(duration, size, 10, 5);
}


struct timestat {
    prectime min;
    prectime max;
    double avg;
};

struct timestat gettimestat(const prectime *times, size_t len) {
    prectime min = UINT64_MAX;
    prectime max = 0;
    double total = 0;    
    
    for (size_t i = 0; i < len; i++) {
        total += times[i];
        if (min > times[i]) min = times[i];
        if (max < times[i]) max = times[i];
    }
    double avg = total/len;
    
    
    struct timestat returnvalue;
    returnvalue.min = min;
    returnvalue.max = max;
    returnvalue.avg = avg;
    return returnvalue;
}

int fprint_timestat(FILE* stream, struct timestat ts) {
    fprintf(stream, "min: %"PRIu64"\n", ts.min);
    fprintf(stream, "max: %"PRIu64"\n", ts.max);
    fprintf(stream, "avg: %f\n", ts.avg);
    return 0;
}

int main() {
//    fprintf(stderr,"starting the control\n");   //<------------------------------------------------------
    struct report datum[4];
    datum[0] = pureload(REQUESTS, 1000);
    datum[1] = heavyload(REQUESTS, 1000);
    datum[2] = mediumload(REQUESTS, 1000);
    datum[3] = lightload(REQUESTS, 1000);
    
    struct timestat malloc_timestats[4];
    struct timestat free_timestats[4];
    for (int i = 0; i < 4; i++) {
        malloc_timestats[i] = gettimestat(datum[i].malloc_elapsedtimes, datum[i].met_len);
        free_timestats[i] = gettimestat(datum[i].free_elapsedtimes, datum[i].fet_len);
    }

    fprintf(stderr, "pureload: %p -- %p\n", datum[0].startaddr, datum[0].endaddr);
    fprintf(stderr, "time: %"PRIu64".%"PRIu64"s\n", datum[0].total_time/1000000, datum[0].total_time%1000000);
    fprintf(stderr, "malloc timestat: \n");
    fprint_timestat (stderr, malloc_timestats[0]);
    fprintf(stderr, "free timestat: \n");
    fprint_timestat (stderr, free_timestats[0]);
    fprintf(stderr, "heavyload: %p -- %p\n", datum[1].startaddr, datum[0].endaddr);
    fprintf(stderr, "time: %"PRIu64".%"PRIu64"s\n", datum[1].total_time/1000000, datum[1].total_time%1000000);
    fprintf(stderr, "malloc timestat: \n");
    fprint_timestat (stderr, malloc_timestats[1]);
    fprintf(stderr, "free timestat: \n");
    fprint_timestat (stderr, free_timestats[1]);
    fprintf(stderr, "mediumload: %p -- %p\n", datum[2].startaddr, datum[1].endaddr);
    fprintf(stderr, "time: %"PRIu64".%"PRIu64"s\n", datum[2].total_time/1000000, datum[2].total_time%1000000);
    fprintf(stderr, "malloc timestat: \n");
    fprint_timestat (stderr, malloc_timestats[2]);
    fprintf(stderr, "free timestat: \n");
    fprint_timestat (stderr, free_timestats[2]);
    fprintf(stderr, "lightload: %p -- %p\n", datum[3].startaddr, datum[2].endaddr);
    fprintf(stderr, "time: %"PRIu64".%"PRIu64"s\n", datum[3].total_time/1000000, datum[3].total_time%1000000);
    fprintf(stderr, "malloc timestat: \n");
    fprint_timestat (stderr, malloc_timestats[3]);
    fprintf(stderr, "free timestat: \n");
    fprint_timestat (stderr, free_timestats[3]);
}
