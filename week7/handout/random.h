#pragma once

#include <stdint.h>

// Facilities for random number generation. You do not have to understand how
// these are implemented, but you need to have an understanding of how to use
// them.

// A random number state. Whenever we wish to generate something random, we will
// be modifying the state.
struct rng {
  uint32_t s;
};

// Initialise the RNG with some random seed.
void seed_rng(struct rng *rng, int seed);

// Generate a random integer in some unspecified range.
int random_int(struct rng *rng);

// Generate a random double in the range (0,1).
double random_double(struct rng *rng);

// Generate a random vector with all components in the range (-1,1).
struct vec random_vec(struct rng *rng);

// Generate a random vector in the unit sphere, meaning it has a length less
// than 1.
struct vec random_in_unit_sphere(struct rng *rng);
