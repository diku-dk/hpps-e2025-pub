CFLAGS=-Wall -Wextra -pedantic -std=c23

all: roll mynameis repeatme noAs test_bitvec8

roll: roll.c
	gcc roll.c -o roll $(CFLAGS)

mynameis: mynameis.c
	gcc mynameis.c -o mynameis $(CFLAGS)

repeatme: repeatme.c
	gcc repeatme.c -o repeatme $(CFLAGS)

noAs: noAs.c
	gcc noAs.c -o noAs $(CFLAGS)

test: test_bitvec8.c bitvec8.c bitvec8.h
	gcc -o test_bitvec8 test_bitvec8.c bitvec8.c $(CFLAGS)

clean:
	rm -f roll mynameis repeatme noAs test_bitvec8 bitvec8_perf
