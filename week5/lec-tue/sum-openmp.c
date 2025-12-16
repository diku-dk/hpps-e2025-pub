#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  assert(argc == 2);
  int n = atoi(argv[1]);

  long long int sum = 0;
#pragma omp parallel for reduction(+:sum)
  for (int i = 0; i < n; i++) {
    sum += i;
  }
  printf("%lld\n", sum);
}
