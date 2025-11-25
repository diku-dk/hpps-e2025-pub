CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic

test: fp8.c test_fp8.c
	$(CC) $(CFLAGS) fp8.c test_fp8.c -o test_fp8

clean:
	rm -f test_fp8 *.o
