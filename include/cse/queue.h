/* queue.h: CSE Queue */

#ifndef CSE_QUEUE_H
#define CSE_QUEUE_H

#include "cse/process.h"

#include <stdio.h>

typedef struct __Queue {
    Process *head;
    Process *tail;
    size_t size;
} Queue;

void queue_push(Queue *q, Process *p);
Process *queue_pop(Queue *q);
Process *queue_remove(Queue *q, pid_t pid);
void queue_dump(Queue *q, FILE *fs);

#endif
