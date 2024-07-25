/* timestamp.c: CSE Timestamp */

#include "cse/timestamp.h"

#include <sys/time.h>
#include <time.h>

/**
 * Return current timestamp as a double.
 *
 * Utilizes gettimeofday for precision (accounting for seconds and
 * microseconds) and falls back to time (only account for seconds) if that
 * fails.
 *
 * @return  Double representing the current time.
 **/
double timestamp() {
    struct timeval tv;
    if (gettimeofday(&tv, NULL) == 0) {
        return (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
    }

    return (double)time(NULL);
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
