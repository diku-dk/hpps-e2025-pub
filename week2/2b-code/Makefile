CC ?= gcc
CFLAGS i?= -std=c2x -Wall -Wextra -Werror -pedantic

# A multiline string using \ to mark the continuation to the next line
PROGRAMS = ex \
		   ptr \
		   print_number \
		   mem \
		   io \
		   fread_bytes

# all label makes it possible to call make without argument.
# calling 'make' is the same as calling 'make all'.
all: $(PROGRAMS)

# % does pattern matching with the label name. The line below indicates that
# the dependencies are restricted to the C file of the same name as the label.
# $^ is the list of files in the dependencies.
# $@ is the label 
% : %.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(PROGRAMS)
