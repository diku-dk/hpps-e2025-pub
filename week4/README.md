# Performance and the memory hierarchy

## Reading

Tuesday:

* HPPS 7

Thursday:

* HPPS - 8

The following are two chapters on the core OS concepts we will be
examining. They are part of a free textbook available online
[here](https://pages.cs.wisc.edu/~remzi/OSTEP/).

* [Processes](https://pages.cs.wisc.edu/~remzi/OSTEP/cpu-intro.pdf)
* [Virtual Memory](https://pages.cs.wisc.edu/~remzi/OSTEP/vm-intro.pdf)

## Videos

* [Caches](https://sid.erda.dk/share_redirect/AjdMSpVIjr/videos/3-l-1/caches.mp4)
* [Locality-aware programming](https://sid.erda.dk/share_redirect/AjdMSpVIjr/videos/3-l-1/locality.mp4)
* [Processes](https://sid.erda.dk/share_redirect/AjdMSpVIjr/videos/3-l-2/intro-and-processes.mp4)
* [Process control](https://sid.erda.dk/share_redirect/AjdMSpVIjr/videos/3-l-2/process-control.mp4)
* [Virtual memory](https://sid.erda.dk/share_redirect/AjdMSpVIjr/videos/3-l-2/virtual-memory.mp4)
* [How to solve address translation exercises](https://sid.erda.dk/share_redirect/AjdMSpVIjr/videos/3-l-2/address-translation-example.mp4)

(Some video players - e.g. Safari on macOS - may have trouble playing
the audio.  If this happens, download the video files and try another
player.)

## Slides

* [Tuesday](4a.pdf)

* [Thursday](4b.pdf)

## Exercises: Locality and Performance

Based on exercises by: Michael Kirkedal Thomsen <kirkedal@acm.org>

Consider the following program.

```
/* You might want to use different values for L,M,N when performing benchmarks. */
#define L 10
#define M 10
#define N 10

long sum_array_3d(long a[L][M][N]) {
  long sum = 0;
  int i, j, k;
  for (i = 0; i < M; i++) {
    for (j = 0; j < N; j++) {
      for (k = 0; k < L; k++) {
        sum += a[k][i][j];
      }
    }
  }
  return sum;
}
```

There is a good reference for how to use 3d arrays here on
[stackoverflow](https://stackoverflow.com/questions/40845302/passing-three-dimensional-arrays-to-a-function-in-c).

### Locality

Sketch what the structure of a 3D array stored in row-major order will be like
in the 1D memory of the computer. What should be the order of the indices be to
achieve spatial locality?

<details>
<summary>Open this to see the answer</summary>

If we suppose that the array elements are *(0,0,0), (0,0,1), ...* (that is,
lexicographically ordered by their index), then a 3D array of shape `[L][M][N]`
may have its elements stored as

```
(0,0,0)
(0,0,1)
...
(0,0,N-1)
(0,1,0)
(0,1,1)
...
(0,M-1,N-1)
(1,0,0)
(1,0,1)
...
(L-1,M-1,N-1)
```

In order to achieve good spatial locality, the innermost indexes must be the
most frequently varying.

</details>

### Performance

Write the above into a C program that you can run. You can base it on the
previous examples. You will need to allocate and initialise a test array
yourself.

Test with different ordering of the indices (and array sizes) to verify your
previous result.

Experiment with how performance varies as you change the compiler
optimisation flags (`-O0`, `-O1`, `-O2`, `-O3`).

Take care to prevent the compiler from completely optimizing away your
computation.  (E.g., print out the sum.)

## Exercise: Working with Data

This set of exercises is intended to prepare you for A4. We will be
working with a dataset of *place name* records provided by
OpenStreetMap, representing 21 million locations on earth. Each record
contains various labeled pieces of information, including an `osm_id`
(a unique integer identifying the place), `lon`/`lat` (decimal numbers
identifying the coordinates), and `name` (the name as a human-readable
string).

We can download a compressed file containing records with the
following command:

```
$ wget https://github.com/OSMNames/OSMNames/releases/download/v2.0.4/planet-latest_geonames.tsv.gz
```

We can then uncompress it:

```
$ gunzip planet-latest_geonames.tsv.gz
```

Note that the uncompressed file is 6.4GiB - make sure you have enough
free space. Each line in the uncompressed file represents one record,
and different fields are separated by spaces.

For testing, such a big file is unwieldy (and slow). The following
command creates a smaller file containing only the first 1000 records:

```
$ head -n 1001 planet-latest_geonames.tsv > 1000records.tsv
```

You can go ahead and open this file in a text editor to get a feel for
it.

### Traversing records

The files
[code/record.c](code/record.c)/[code/record.h](code/record.h) form a
simple library for reading records from a data file. Read the comments
`.h` file to see how to use it. You do not need to read or understand
the `.c` file.  You certainly should not modify it.

Based on what you have learned (and can remember) about how to
structure C programs, write a program `names` that does the following:

* Uses `read_records` to read the records from a given file name.

* Prints the `name` field of each record.

* Cleans up after itself by freeing data as needed.

It is up to you how you structure your program, and how you compile
it - e.g. whether you create a `Makefile`, and how it works. But there
is no reason not to do a good job.

#### Example usage

```
$ solution/names 1000records.tsv
France
United States of America
Germany
Italy
Japan
Russia
Spain
Poland
Canada
England
...
```

#### Hints

* Remember to `#include "record.h"` in the file containing your `main`
  function.

* The `read_records()` function returns an array (even though it looks
  like a pointer - remember that arrays are represented as a pointer
  to their first element).

* If `rs` is an array of records, then use `rs[i].name` to access the
  `name` field of the `i`th element.

[Solution here.](solution/)

## Exercise: Investigating System Calls

1. Take a program that performs some operations - your solutions to A2 are a
good case, or write a small *hello world* program. Run the program with `strace`
to get a list of system calls performed during execution.

2. Find the first system call that corresponds to code that *you* wrote - the
first (many) will be system calls performed during initialisation code inserted
by the compiler. It can help to insert a `printf()` with a known string as the
first thing in the `main()` function, and then looking for that in the `strace
output.

3. Determine which system calls correspond to which parts of your program. Try
   to run your program with different arguments to see how the behaviour
   changes - in particular, try to see what happens when an `assert()` fails.

## Exercise: Browse processes

Use `top` to investigate the running processes. You may also wish to use `htop`
which may have a more easily read UI. Investigate how many processes are
running, even when you only have your process monitoring application is open,
how many are open when you start your regular browser/music player/social media
apps etc.

## Exercise: Use up memory

Write a program that will claim all of your virtual memory. You may have to
investigate how much virtual memory your device has. Your program should
continuously access this memory.

Run several instances of your program at the same time. How many do you need to
run before you start seeing performance drop? Can you conclude anything from
this about your hardware?
