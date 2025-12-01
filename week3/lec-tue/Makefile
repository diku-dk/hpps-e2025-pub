plain: collatz.c
	gcc collatz.c -o collatz

optim: collatz.c
	gcc -O3 -ffast-math collatz.c -o o-collatz

object: collatz.c
	gcc -c collatz.c -o collatz.o

reloc: collatz.c
	gcc -fPIC -shared collatz.c -o libcollatz.so

run:
	time ./collatz 100000 >/dev/null

optim-run:
	time ./o-collatz 100000 >/dev/null

space:
	/usr/bin/time -l ./o-collatz 100000 > /dev/null

python-run:
	time python3 ./collatz.py 100000 >/dev/null
	
python-space:
	/usr/bin/time -l python3 ./collatz.py 100000 >/dev/null
	
clean:
	rm -f collatz
	rm -f o-collatz
	rm -f collatz.o
	rm -f libcollatz.so
