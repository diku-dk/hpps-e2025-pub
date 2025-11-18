# Week 1 - C and Integers

## Suggested Reading

Tuesday:

* HPPS 1
* JG 1-3

Thursday:

* HPPS 2
* JG 4-7

## Videos

* [Command line arguments](https://sid.erda.dk/share_redirect/f8RgVGzlET/argv.mp4)

## Slides

* [Tuesday (intro)](1intro.pdf)
* [Tuesday](1a.pdf)
* [Thursday](1b.pdf)

## Basic Unix Exercises

### Basic setup and Unix familiarity

You will need access to a Unix shell for HPPS. Read the [Unix software
guide](../unix.md).

After reading and following the guide, open a terminal with a Unix
shell.  When illustrating a shell command, we will write it like this:

```
$ echo HPPS is my favourite course
HPPS is my favourite course
```

The `$` is not part of the input you type, but a stand-in for the
command prompt.  On the line after the command (the `echo ...` part),
we may put the output of the command.  When we tell you to run a
command, we will often not show the output, but merely tell you to run
e.g:

```
$ echo HPPS is the favourite course of $USER
```

If you want a reminder about using a command prompt, then you might
find thousands of tutorials and guides online on how to use a Unix shell, all of
which are a [search engine
query](https://duckduckgo.com/?t=h_&q=how+do+i+unix&ia=web) away. You
do not need to be a command prompt expert to succeed in HPPS, but you
should understand the following:

* The general tree structure of the file system.

* How to move between directories (`cd`) and see their contents (`ls`).

* How to view file contents from a command line (`cat`, `less`, etc).

* How to copy files (`cp`).

* How to move/rename files (`mv`).

* How to delete files and directories (`rm`).

### Compiling C programs

You will be doing a lot of C programming in HPPS, so you need to make
sure you can compile C code.  You are not expected to understand what
the C code *does* yet.

Copy the file [guess.c](guess.c) to a directory on your computer, open
a Unix terminal, and navigate to the directory containing `guess.c`.
Then run:

```
$ gcc guess.c -o guess
```

This will compile the C program and generate a binary called `guess`.
Now run the `./guess` program and complete the game:

```
$ ./guess
OK, I am thinking of a number in the range [0,99] - see if you can guess it!
Type a number and press enter to make a guess.
42
Too high!
...
```

While you don't know C yet, you can still try the following (optional)
exercises to modify the `guess.c` program.  Remember that you have to
re-run the `gcc` command after making a modification!

* Change the range from which the random number is drawn.

* Make the program print a message when the player enter something else than an integer.

* Make the program cheat, so that it changes the number it is thinking
  of when it would otherwise have been guessed.


## C Exercises

The main purpose of the following exercises is to give you experience
with programming in C, as well as prepare you for A1 (the first
mandatory assignment). They mostly consist of setting up a working C
development environment (compiler, editor), which you will need today.

For the coding exercises, you should use `make` to build your
programs. To parametrize `make` to build with all the necessary
compiler flags, start by writing down a `Makefile` containing the
following:

```
CC?=cc
CFLAGS?=-std=c11 -Wall -Werror -Wextra -pedantic
```

Then when you add a program `foo`, add a rule to `Makefile` as
follows:

```
foo: foo.c
	$(CC) -o foo foo.c $(CFLAGS)
```

**Beware**: the leading whitespace *must* be a single tab character,
*not* multiple spaces. If you transcribe it by hand, you may enter the
wrong invisible character (this is not a joke; computers really are
like this).

Now, to compile `foo`, run `make foo`. For example:

```
$ make mynameis
```

There are ways to automate parts of this, but we recommend being
explicit until you get a firm grasp on what `make` does. The
`Makefile`s given to you in HPPS will generally be quite simple.
Understanding the subtle intricacies of `make` is not part of the
course learning goals.

In the following, we colloquially use "print" as slang for writing to
standard output.

* [Code handout here](handout/)
* [Reference solutions here](solution/)

Remember to check the reference solutions once you've written your
own - perhaps they'll show you ways of doing things more concisely.

### 1. Printing in C

Write a C program `mynameis` that prints your name.

### 2. Input arguments in C

Write a C program `repeatme` that given a string as argument, prints
the string twice on separate lines.  (You can read section C.10.3 of Harris & Harris
or maybe watch [the video on
reading line
arguments](https://sid.erda.dk/share_redirect/f8RgVGzlET/argv.mp4).)

If the program gets a number of arguments that is different from 1 it
should print "`Wrong number of arguments`".

### 3. Input argument validation

Write a C program `noAs` that given a string as argument, does either
of the following.

  * If the first character of the string is the `char` `A` it
    prints "`No beginning A's are allowed`",

  * otherwise it prints the given string.

Reminder: Consider how strings are formatted in C.

Note: reuse your argument validation from before. You can just as well
learn it from the beginning: _always check your inputs._

### 4. Multiple input formats

Modify the `roll` program from the handout to support `dN` as
a shorthand for `1dN`.  Hint: if one `sscanf()` fails, try another.

### A1 warmup -- Bits, libraries, and tests in C

This exercise is a preparation for A1.  You will
(a) write a small C library for computing with bit vectors
and (b) a test for each function defined in the library.  A library is
a set of reusable definitions (types and functions) that can be used
by applications.  

Our library consists of a *header file* `bitvec8.h` and an *implementation file*
`bitvec8.c`. In addition, we rely on a *test program* 'test_bitvec8'
to test the library. The test file ¨test_bitvec8.c' uses the definitions of the header
file and tests the correctness of their implementation.A Makefile
is provided to build the *test program*. They are part of the [code
handout](handout/), but are unfinished.

Use `make test` to compile the test program together with the library.

#### bitvec8

The library operates on the type `bitvec8` that represents an 8 bit-vector. 

**Bit Vector: `01010000`**

| Bit # | 7 (MSB) | 6 | 5 | 4 | 3 | 2 | 1 | 0 (LSB) |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| Value | **0** | **1** | **0** | **1** | **0** | **0** | **0** | **0** |

The difficulty is that each bitwise operation takes an entire 8-bit vector
as parameter. *You cannot just operate on a single bit*.

##### Reading code and designing tests

Three functions defined in `bitvec8.h` are already implemented in `bitvec8.c`
and tested in `test_bitvec8.c`.

Note that the implementation does not use explicit control flow (i.e., if or
loop statements).

1. Explain  the technique that is used in `bitvec8_from_int` to truncate the
input number to 8 bits. Add a couple of additional test cases to
`test_bitvec8_from_int`. Considering 'input partitioning', how many
test cases are sufficient to    

2. Explain how iteration is implemented in `bitvec8_print`.
Explain how testing is organized for this function. How is this different
than the testing done for the other two functions?

#### Writing code (functions and tests)

Implement the remaining 7 functions defined and specified in `bitvec8.h` using techniques
introduced in class (week1/1b.pdf). 

Implement a test function for each of these functions. Use input partitioning
to define the test cases.

A test cases calls a function in the library, compares the obtained
result with the expected result, and complains loudly if there is a
discrepancy.  
The recommended way to do this is to use the `assert` macro
defined in the standard `<assert.h>` header.
One downside of using `assert` is that while it will tell you that the
comparison failed, it will not tell you which result was actually
returned.  


### Integer representation

Play the integer representation game linked below. Try to get `0xA`
correct on the different "levels".

http://topps.diku.dk/compsys/integers.html

Next, answer all of the following:

* http://topps.diku.dk/compsys/integers.html#NjdfZmFsc2VfMV8yXzE=
* http://topps.diku.dk/compsys/integers.html#NjFfZmFsc2VfMV8yXzE=
* http://topps.diku.dk/compsys/integers.html#MTAzX2ZhbHNlXzFfMl8x
* http://topps.diku.dk/compsys/integers.html#OTBfZmFsc2VfMV8yXzE=
* http://topps.diku.dk/compsys/integers.html#MjU1X2ZhbHNlXzFfMF8y
* http://topps.diku.dk/compsys/integers.html#MTUzX2ZhbHNlXzFfMF8y
* http://topps.diku.dk/compsys/integers.html#MjE5X2ZhbHNlXzFfMF8x
* http://topps.diku.dk/compsys/integers.html#OTBfZmFsc2VfMV8wXzE=

### Integer puzzles

Given the following initialization:

```C
#include "stdint"

int32_t x = foo();
int32_t y = bar();
uint32_t ux = x;
uint32_t uy = y;
```

Consider whether each of the following statements hold:

* `x < 0` => `((x*2) < 0)`
* `ux >= 0`
* `x & 7 == 7`  => `(x<<30) < 0`
* `ux > -1`
* `x > y`   =>  `-x < -y`
* `x * x >= 0`
* `x > 0 && y > 0`  =>  `x + y > 0`
* `x >= 0`  =>  `-x <= 0`
* `x <= 0`  =>  `-x >= 0`
* `(x|-x)>>31 == -1`
* `ux >> 3 == ux/8`
* `x >> 3 == x/8`
* `x & (x-1) != 0`
