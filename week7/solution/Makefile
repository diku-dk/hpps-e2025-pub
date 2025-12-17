CFLAGS?=-Wall -Wextra -pedantic -g -O -lm -fsanitize=address
CC?=cc
EXECUTABLES=simple_test test_describe_object roll_dice
OBJECTS=scene.o geometry.o random.o

# Disable bultin rules.
.SUFFIXES:

all: $(EXECUTABLES)

simple_test: simple_test.o $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

test_describe_object: test_describe_object.o $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

roll_dice: roll_dice.o $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm -rf $(EXECUTABLES) *.o
