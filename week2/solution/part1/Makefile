# This is a Makefile comment.
# ?= is a conditional assignment. If CC is not already initialized, it gets the value cc.
# CC and CFLAGS are so-called environment variables. They are accessed via $(CC) and $(CFLAGS)
CC?=cc
CFLAGS?=-std=c11 -Wall -Werror -Wextra -pedantic

# label : dependencies
# (tab) command
# The label can be passed as a parameter to make, e.g., make test_numlib
# If none of the files in the dependencies has changed, then the command is not executed.
# As in F# or Python, indentation is crucial to mark the boundaries of a command.
# Indentation MUST be a tab.
# The command is any shell command.
test_numlib: test_numlib.c numlib.c
	$(CC) -o test_numlib test_numlib.c numlib.c $(CFLAGS)

clean:
	rm test_numlib

