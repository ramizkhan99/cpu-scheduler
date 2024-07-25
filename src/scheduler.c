/* scheduler.c: CSE Scheduler */

#include "cse/scheduler.h"
#include "cse/macros.h"
#include "cse/process.h"
#include "cse/queue.h"
#include "cse/timestamp.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

/**
 * Add new command to waiting queue.
 * @param   s	    Pointer to Scheduler structure.
 * @param   fs      File stream to write to.
 * @param   command Command string for new Process.
 **/
void scheduler_add(Scheduler *s, FILE *fs, const char *command) {
    Process *p = process_create(command);
    if (p == NULL) {
        error("failed to create process");
        return;
    }
    queue_push(&s->waiting, p);
    for (Process *tmp = s->waiting.head; tmp != NULL; tmp = tmp->next) {
        fprintf(fs, "%d ", tmp->pid);
    }
}

/**
 * Display status of queues in Scheduler.
 * @param   s	    Pointer to Scheduler structure.
 * @param   fs      File stream to write to.
 * @param   queue   Bitmask specifying which queues to display.
 **/
void scheduler_status(Scheduler *s, FILE *fs, int queue) {
    fprintf(
        fs,
        "Running = %4lu, Waiting = %4lu, Finished = %4lu, Turnaround = "
        "%05.2lf, Response = %05.2lf\n",
        s->running.size, s->waiting.size, s->finished.size,
        s->finished.size == 0 ? 0 : s->total_turnaround_time / s->finished.size,
        s->finished.size == 0 && s->running.size == 0
            ? 0
            : s->total_response_time / (s->running.size + s->finished.size));
}

/**
 * Schedule next process using appropriate policy.
 * @param   s	    Pointer to Scheduler structure.
 **/
void scheduler_next(Scheduler *s) {
    switch (s->policy) {
    case FIFO_POLICY:
        scheduler_fifo(s);
        break;
    case RDRN_POLICY:
        scheduler_rdrn(s);
        break;
    }
}

/**
 * Wait for any children and remove from queues and update metrics.
 * @param   s	    Pointer to Scheduler structure.
 **/
void scheduler_wait(Scheduler *s) {
    pid_t pid;
    while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
        Process *p = queue_remove(&s->running, pid);
        if (p == NULL) {
            error("Process with pid %d not found in running queue", pid);
            continue;
        }
        p->end_time = timestamp();
        s->total_turnaround_time += p->end_time - p->arrival_time;
        queue_push(&s->finished, p);
    }
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
