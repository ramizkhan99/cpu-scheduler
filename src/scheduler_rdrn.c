/* scheduler_rdrn.c: CSE Round Robin Scheduler */

#include "cse/macros.h"
#include "cse/process.h"
#include "cse/queue.h"
#include "cse/scheduler.h"

#include <assert.h>

/**
 * Schedule next process using round robin policy:
 *
 *  1. If there no waiting processes, then do nothing.
 *
 *  2. Move one process from front of running queue and place in back of
 *  waiting queue.
 *
 *  3. Move one process from front of waiting queue and place in back of
 *  running queue.
 *
 * @param   s	    Scheduler structure
 **/
void scheduler_rdrn(Scheduler *s) {
    if (s->waiting.size == 0) {
        return;
    }

    Process *waiting = queue_pop(&s->waiting);
    Process *running = queue_pop(&s->running);

    if (waiting != NULL && process_resume(waiting)) {
        queue_push(&s->running, waiting);
    }

    if (running != NULL && process_pause(running)) {
        queue_push(&s->waiting, running);
    }
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
