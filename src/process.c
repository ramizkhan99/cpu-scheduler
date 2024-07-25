/* process.c: CSE Process */

#include "cse/process.h"
#include "cse/macros.h"
#include "cse/timestamp.h"

#include <complex.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

/**
 * Create new process structure given command.
 * @param   command     String with command to execute.
 * @return  Pointer to new process structure
 **/
Process *process_create(const char *command) {
    Process *p = (Process *)malloc(sizeof(Process));
    strcpy(p->command, command);
    p->pid = 0;
    p->next = NULL;
    p->arrival_time = 0;
    p->start_time = 0;
    p->arrival_time = timestamp();
    return p;
}

/**
 * Start process by forking and executing the command.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not starting the process was successful
 **/
bool process_start(Process *p) {
    p->pid = fork();

    if (p->pid == 0) {
        // Child process
        char *argv[MAX_ARGUMENTS] = {0};
        int i = 0;
        char *token = strtok(p->command, " ");
        while (token != NULL && i < MAX_ARGUMENTS) {
            argv[i++] = token;
            token = strtok(NULL, " ");
        }
        argv[i] = NULL;

        if (execvp(argv[0], argv) == -1) {
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }
    } else if (p->pid > 0) {
        // Parent process
        p->start_time = timestamp();
    } else {
        // Fork failed
        perror("fork failed");
        return false;
    }

    return true;
}

/**
 * Pause process by sending it the appropriate signal.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not sending the signal was successful.
 **/
bool process_pause(Process *p) {
    if (kill(p->pid, SIGSTOP) == -1) {
        perror("failed to pause program");
        return false;
    }
    return true;
}

/**
 * Resume process by sending it the appropriate signal.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not sending the signal was successful.
 **/
bool process_resume(Process *p) {
    if (kill(p->pid, SIGCONT) == -1) {
        perror("failed to resume program");
        return false;
    }
    return true;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
