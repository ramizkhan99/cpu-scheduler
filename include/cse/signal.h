#ifndef CSE_SIGNAL_H
#define CSE_SIGNAL_H

#include <signal.h>
#include <stdbool.h>

#ifndef GNU_SOURCE
typedef void (*sighandler_t)(int);
#endif

bool register_signal(int signum, int flags, sighandler_t handler);
void sigalrm_handler(int signum);

#endif
