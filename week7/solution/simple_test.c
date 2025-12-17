// Compile with:
//
//  gcc -Wall -Wextra -pedantic -lm simple_test.c geometry.c scene.c -o simple_test
//
// Or write a proper Makefile. (One is actually included in the repository.)

#include "geometry.h"
#include "scene.h"

int main(void) {
  struct material m;
  m.type = METAL;
  m.metal.albedo.x = 1.0;
  m.metal.albedo.y = 0.5;
  m.metal.albedo.z = 0.3;
  m.metal.fuzz = 0.8;

  // Hide unused-variable warning;
  (void)m;
}
