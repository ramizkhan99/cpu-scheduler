/* options.h: Command line options */

#ifndef CSE_OPTIONS_H
#define CSE_OPTIONS_H

#include "cse/scheduler.h"

#include <stdbool.h>

bool parse_command_line_options(int argc, char *argv[], Scheduler *s);

#endif
