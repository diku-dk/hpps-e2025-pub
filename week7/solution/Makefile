CFLAGS?=-Wall -Wextra -pedantic -g -O -lm -fsanitize=address
CC?=cc
EXECUTABLES=simple_test test_describe_object

# Disable bultin rules.
.SUFFIXES:

all: $(EXECUTABLES)

simple_test: simple_test.o geometry.o scene.o
	$(CC) -o $@ $^ $(CFLAGS)

test_describe_object: test_describe_object.o geometry.o scene.o
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm -rf $(EXECUTABLES) *.o
