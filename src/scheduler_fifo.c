/* scheduler_fifo.c: CSE FIFO Scheduler */

#include "cse/macros.h"
#include "cse/scheduler.h"
#include "cse/timestamp.h"

#include <assert.h>

/**
 * Schedule next process using fifo policy:
 *
 *  As long as we have less running processes than our number of CPUS and
 *  there are waiting processes, then we should move a process from the
 *  waiting queue to the running queue.
 *
 * @param   s	    Scheduler structure
 */
void scheduler_fifo(Scheduler *s) {
    if (s->running.size == s->cores)
        return;

    Process *p = queue_pop(&s->waiting);
    if (p == NULL) {
        error("failed to pop from waiting queue");
        return;
    }

    if (!process_start(p)) {
        error("failed to start process\n");
        return;
    }

    p->start_time = timestamp();
    s->total_response_time += p->start_time - p->arrival_time;
    queue_push(&s->running, p);
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
