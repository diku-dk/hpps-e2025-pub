#include "random.h"
#include "geometry.h"

static const uint32_t a = 48271;
static const uint32_t c = 0;
static const uint32_t m = 2147483647U;

void seed_rng(struct rng *rng, int seed) {
  rng->s = seed ^ 5461;
  (void)random_int(rng);
  (void)random_int(rng);
  (void)random_int(rng);
  (void)random_int(rng);
}

int random_int(struct rng *rng) {
  rng->s = a  *rng->s + c % m;
  return rng->s;
}

double random_double(struct rng *rng) {
  return (double)random_int(rng) / (double)m;
}


struct vec random_vec(struct rng *rng) {
  double x = random_double(rng) * 2 - 1;
  double y = random_double(rng) * 2 - 1;
  double z = random_double(rng) * 2 - 1;
  return (struct vec){.x = x, .y = y, .z = z};
}

struct vec random_in_unit_sphere(struct rng *rng) {
  struct vec v = random_vec(rng);
  while (vec_quadrance(v) >= 1) {
    v = random_vec(rng);
  }
  return v;
}
