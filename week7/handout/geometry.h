#pragma once

// General facilities for geometrical calculations. You do not have to
// understand how these functions work (i.e., you do not have to read
// geometry.c), but it can be useful to understand what they do.

// A three-dimensional vector.
struct vec { double x, y, z; };

// Scale a vector by a scalar, i.e., multiply every component of 'v' with 't'.
struct vec vec_scale(double t, struct vec v);

// Negate a vector.
struct vec vec_neg(struct vec v);

// Add together two vectors, producing a new vector.
struct vec vec_add(struct vec a, struct vec b);

// Multiply elementwise two vectors, producing a new vector.
struct vec vec_mul(struct vec a, struct vec b);

// Subtract two vectors, producing a new vector.
struct vec vec_sub(struct vec a, struct vec b);

// The dot product of two vectors.
double vec_dot(struct vec a, struct vec b);

// The cross product of two vectors.
struct vec vec_cross(struct vec a, struct vec b);

// The quadrance of a vector. This is the square of the norm.
double vec_quadrance(struct vec v);

// The norm (length) of a vector.
double vec_norm(struct vec v);

// Normalise a vector such that it has length 1, but the same direction as the
// original vector.
struct vec vec_normalise(struct vec v);

// Generate a random vector.
struct vec random_vec();

// Generate a random vector in the unit sphere, meaning it has a length less
// than 1.
struct vec random_in_unit_sphere();

// A ray through space is a combination of an origin and a normalised direction
// (in contrast to plain vectors, which in some sense all originate at (0,0,0)).
struct ray {
  struct vec origin;
  struct vec direction;
};

// Follow the given ray 'r' from its starting point along its direction for
// distance 't', returning the resulting point.
struct vec point_at_parameter(struct ray r, double t);

// Reflect a vector by a different vector.
struct vec reflect(struct vec v, struct vec n);

// Axis-aligned bounding box. This is a box where all axes are parallel to an
// axis (x,y,z). The representation is by two corners of the box: the one with
// the lowest-valued components, and the one with the highest-valued components.
struct aabb {
  struct vec min, max;
};

// Construct the smallest AABB that contains two given boxes.
struct aabb aabb_enclosing(const struct aabb *box0, const struct aabb *box1);

// Find the centre of an AABB.
struct vec aabb_centre(const struct aabb *a);
