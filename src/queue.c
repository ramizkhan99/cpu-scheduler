/* queue.c: CSE Queue */

#include "cse/queue.h"
#include "cse/macros.h"
#include "cse/process.h"

#include <assert.h>
#include <stdio.h>

/**
 * Push process to back of queue.
 * @param q     Pointer to Queue structure.
 **/
void queue_push(Queue *q, Process *p) {
    if (q->size == 0) {
        q->head = p;
        q->tail = p;
    } else {
        q->tail->next = p;
        q->tail = q->tail->next;
    }
    q->size++;
}

/**
 * Pop process from front of queue.
 * @param q     Pointer to Queue structure.
 * @return  Process from front of queue.
 **/
Process *queue_pop(Queue *q) {
    if (q->size == 0) {
        error("failed to pop from queue: queue is empty\n");
        return NULL;
    }
    Process *p;
    p = q->head;
    q->head = q->head->next;
    q->size--;
    return p;
}

/**
 * Remove and return process with specified pid.
 * @param q     Pointer to Queue structure.
 * @param pid   Pid of process to return.
 * @return  Process from Queue with specified pid.
 **/
Process *queue_remove(Queue *q, pid_t pid) {
    if (q->size == 0)
        return NULL;

    Process *prev = NULL;
    Process *p = q->head;

    // Traverse the queue to find the process with the specified pid
    while (p != NULL) {
        if (p->pid == pid) {
            // Found the process, unlink it from the queue
            if (prev == NULL) {
                // Removing the head of the queue
                q->head = p->next;
                if (q->tail == p) {
                    // If p is also the tail, update q->tail
                    q->tail = NULL;
                }
            } else {
                // Removing from somewhere in the middle or end of the queue
                prev->next = p->next;
                if (q->tail == p) {
                    // If p is the tail, update q->tail
                    q->tail = prev;
                }
            }
            q->size--;
            return p;
        }
        prev = p;
        p = p->next;
    }

    error("Process with pid %d not found in queue\n", pid);
    return NULL;
}

/**
 * Dump the contents of the Queue to the specified stream.
 * @param q     Queue structure.
 * @param fs    Output file stream.
 **/
void queue_dump(Queue *q, FILE *fs) {
    fprintf(fs, "%6s %-30s %-13s %-13s %-13s\n", "PID", "COMMAND", "ARRIVAL",
            "START", "END");
    /* TODO: Display information for each item in Queue. */
}
