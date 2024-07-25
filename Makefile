# Configuration

CC		= gcc
LD		= gcc
AR		= ar
CFLAGS		= -g -std=gnu99 -Wall -Iinclude -fPIC #-DDEBUG=1
LDFLAGS		= -Llib
ARFLAGS		= rcs

# Variables

LIBRARY_HEADERS = $(wildcard include/cse/*.h)
LIBRARY_SOURCES = src/options.c src/process.c src/queue.c src/signal.c \
		  src/scheduler.c src/scheduler_fifo.c src/scheduler_rdrn.c \
		  src/timestamp.c
LIBRARY_OBJECTS	= $(LIBRARY_SOURCES:.c=.o)
STATIC_LIBRARY  = lib/cse.lib
CSE_PROGRAM	= bin/cse

# Rules

all:	$(CSE_PROGRAM)

%.o:	%.c $(LIBRARY_HEADERS)
	@echo "Compiling $@"
	@$(CC) $(CFLAGS) -c -o $@ $<

$(CSE_PROGRAM):	src/cse.o $(STATIC_LIBRARY)
	@echo "Linking $@"
	@$(LD) $(LDFLAGS) -o $@ $^

$(STATIC_LIBRARY):	$(LIBRARY_OBJECTS)
	@echo "Linking $@"
	@$(AR) $(ARFLAGS) $@ $^

clean:
	@echo "Removing objects"
	@rm -f $(LIBRARY_OBJECTS) src/*.o

	@echo "Removing static library"
	@rm -f $(STATIC_LIBRARY)

	@echo "Removing CSE program"
	@rm -f $(CSE_PROGRAM)

.PRECIOUS: %.o
