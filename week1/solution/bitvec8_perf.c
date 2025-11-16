#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "bitvec8.h"

int main() {
  const int N = 1000000;
  uint8_t vectors[N];

  // Initialize with different bit patterns
  for (int i = 0; i < N; i++) {
    vectors[i] = (uint8_t)i;
  }

  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);

  volatile int sum = 0;
  for (int i = 0; i < N; i++) {
    sum += bitvec8_popcount(vectors[i]);
  }

  clock_gettime(CLOCK_MONOTONIC, &end);

  double elapsed = (end.tv_sec - start.tv_sec) +
                   (end.tv_nsec - start.tv_nsec) / 1e9;

  printf("Time: %.6f seconds\n", elapsed);
  printf("Total popcount: %d\n", sum);

  return 0;
}
