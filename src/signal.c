#include "cse/signal.h"
#include "cse/macros.h"
#include "cse/scheduler.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern Scheduler ShellScheduler;

/**
 * Register signal handler with specified flags.
 *
 * @param   signum      Signal number
 * @param   flags       Signal action flags
 * @param   handler     Signal handler
 * @return  Whether or not the registration was successful.
 **/
bool register_signal(int signum, int flags, sighandler_t handler) {
    struct sigaction action = {{0}};
    action.sa_handler = handler;
    action.sa_flags = flags;
    sigemptyset(&action.sa_mask);
    if (sigaction(signum, &action, NULL) < 0) {
        error("Failed to register signal: %s", strerror(errno));
        return false;
    }
    return true;
}

/**
 * SIGALRM handler.
 *
 * @param   signum      Signal number.
 **/
void sigalrm_handler(int signum) {
    if (ShellScheduler.running.size > 0)
        scheduler_wait(&ShellScheduler);

    if (ShellScheduler.waiting.size > 0)
        scheduler_next(&ShellScheduler);
}
