#ifndef TIMEMALLOC_H
#define TIMEMALLOC_H

#include "prectime.h"

#include <stddef.h>

struct mt_report {
    prectime mt_time;
    void *addr;
};


/* function: time_esmalloc(size_t size)
 * Wrapper function around the library malloc function
 * to measure the time taken to execute the malloc call.
 *
 * inputs
 * size: takes the number of bytes to be allocated in 
 *       the heap (required to execute the malloc)and
 *       just passes it off to the esmalloc.
 *
 * returns
 * a report structure which contains time taken to execute 
 * the malloc call and the address in heap returned by the 
 * malloc call. Address value is also returned so that it
 * could be used to free the memory blocks
 */
struct mt_report timemalloc(size_t size);

/* function: time_esfree():
 * Wrapper function around the library free function
 * to measure the time taken to execute the free call.
 *
 * returns
 * time taken (in microseconds) to execute the free call
 * */
prectime timefree(void *addr);

#endif
