/* cse.c: Process Queue Shell */

#include "cse/macros.h"
#include "cse/options.h"
#include "cse/scheduler.h"
#include "cse/signal.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

/* Global Variables */

Scheduler ShellScheduler = {
    .policy = FIFO_POLICY,
    .cores = 1,
    .timeout = 250000,
};

/* Help Message */

void help() {
    printf("Commands:\n");
    printf("  add    command    Add command to waiting queue.\n");
    printf("  status [queue]    Display status of specified queue (default is "
           "all).\n");
    printf("  help              Display help message.\n");
    printf("  exit|quit         Exit shell.\n");
}

int main(int argc, char *argv[]) {
    Scheduler *s = &ShellScheduler;

    /* Parse command line options */
    if (argc > 1 && !parse_command_line_options(argc, argv, s)) {
        error("failed to parse command line options\n");
        exit(1);
    }

    /* Register signal handlers */
    if (!register_signal(SIGALRM, NULL, sigalrm_handler)) {
        error("failed to register alarm signal");
        exit(1);
    }

    /* Start timer interrupt */
    struct itimerval interval = {
        .it_interval = {.tv_sec = 0, .tv_usec = s->timeout},
        .it_value = {.tv_sec = 0, .tv_usec = s->timeout},
    };

    if (setitimer(ITIMER_REAL, &interval, NULL) < 0) {
        error("failed to start timer: %s\n", strerror(errno));
        exit(1);
    }

    /* Process shell comands */
    while (!feof(stdin)) {
        char command[BUFSIZ] = "";

        printf("\nCSE> ");

        while (!fgets(command, BUFSIZ, stdin) && !feof(stdin))
            ;

        chomp(command);

        /* Handle add and status commands */
        if (streq(command, "help")) {
            help();
        } else if (streq(command, "exit") || streq(command, "quit")) {
            break;
        } else if (strlen(command)) {
            char *token = strtok(command, " ");
            if (streq(token, "add")) {
                scheduler_add(s, stdin, strtok(NULL, ""));
                continue;
            } else if (streq(token, "status")) {
                token = strtok(NULL, " ");
                if (!token || streq(token, "all")) {
                    scheduler_status(s, stdout, WAITING);
                    scheduler_status(s, stdout, RUNNING);
                    scheduler_status(s, stdout, FINISHED);
                } else if (streq(token, "waiting")) {
                    scheduler_status(s, stdout, WAITING);
                } else if (streq(token, "running")) {
                    scheduler_status(s, stdout, RUNNING);
                } else if (streq(token, "finished")) {
                    scheduler_status(s, stdout, FINISHED);
                } else {
                    printf("Unknown scheduler queue: %s\n", token);
                }
                continue;
            }
            printf("Unknown command: %s\n", command);
        }
    }

    return EXIT_SUCCESS;
}
