/* process.h: Process structure */

#ifndef CSE_PROCESS_H
#define CSE_PROCESS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ARGUMENTS 1024

typedef struct __Process Process;
struct __Process {
    char command[BUFSIZ]; // to execute
    pid_t pid;            // Process identifier (0 == invalid)
    double arrival_time;  // arrival time (is placed into waiting queue)
    double start_time;    // start time (is first placed into running queue)
    double end_time;      // end time (is placed into finished queue)

    Process *next; // Pointer to next process
};

Process *process_create(const char *command);
bool process_start(Process *p);
bool process_pause(Process *p);
bool process_resume(Process *p);

#endif
