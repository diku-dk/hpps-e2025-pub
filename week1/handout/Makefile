CFLAGS = -Wall -Wextra -Werror -pedantic -std=c11

guess: guess.c
	gcc $(CFLAGS) guess.c -o guess

roll: roll.c
	gcc $(CFLAGS) roll.c -o roll

test: test_bitvec8.c bitvec8.c bitvec8.h
	gcc $(CFLAGS) test_bitvec8.c bitvec8.c -o test_bitvec8 

clean: 
	rm -f guess
	rm -f roll
	rm -f test_bitvec8
