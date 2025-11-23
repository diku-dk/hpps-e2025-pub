CC?=cc
CFLAGS?=-std=c11 -Wall -Werror -Wextra -pedantic

test_numlib: test_numlib.c numlib.c
	$(CC) -o test_numlib test_numlib.c numlib.c $(CFLAGS)

clean:
	rm test_numlib
