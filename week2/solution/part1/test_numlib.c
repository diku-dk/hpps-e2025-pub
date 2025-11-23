#include "numlib.h"
#include <stdio.h>
#include <assert.h>

void test_get_number(void) {
  int n = get_number();
  assert(n == 42);
  printf("%d\n", n);
  
}

int main(void) {
  test_get_number();

  printf("All tests passed.\n");
}
