// Compile with
//
//    gcc roll_dice.c geometry.c random.c -o roll_dice -lm
//
// Or use the Makefile.

#include "random.h"
#include "scene.h"

int main(void) {
  int k = 10;
  struct rng rng;
  seed_rng(&rng, 42);

  for (int i = 0; i < k; i++) {
    int roll = random_int(&rng) % 6;
    printf("%d\n", roll);
  }
}
