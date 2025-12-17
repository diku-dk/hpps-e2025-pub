#include "geometry.h"
#include <stdlib.h>
#include <math.h>

struct vec vec_scale(double t, struct vec v) {
  return (struct vec) { .x = t * v.x,
                         .y = t * v.y,
                         .z = t * v.z};
}


struct vec vec_neg(struct vec v) {
  return vec_scale(-1, v);
}


struct vec vec_add(struct vec a, struct vec b) {
  return (struct vec) { .x = a.x + b.x,
                         .y = a.y + b.y,
                         .z = a.z + b.z};
}

struct vec vec_mul(struct vec a, struct vec b) {
  return (struct vec) { .x = a.x * b.x,
                         .y = a.y * b.y,
                         .z = a.z * b.z};
}

struct vec vec_sub(struct vec a, struct vec b) {
  return (struct vec) { .x = a.x - b.x,
                         .y = a.y - b.y,
                         .z = a.z - b.z};
}

double vec_dot(struct vec a, struct vec b) {
  return a.x*b.x + a.y*b.y + a.z*b.z;
}

struct vec vec_cross(struct vec a, struct vec b) {
  return (struct vec) {.x=a.y*b.z-a.z*b.y,
                        .y=a.z*b.x-a.x*b.z,
                        .z=a.x*b.y-a.y*b.x};
}


double vec_quadrance(struct vec v) {
  return vec_dot(v,v);
}

double vec_norm(struct vec v) {
  return sqrt(vec_quadrance(v));
}

struct vec vec_normalise(struct vec v) {
  double l = vec_norm(v);
  return vec_scale(1/l, v);
}

struct vec point_at_parameter(struct ray r, double t) {
  return vec_add(r.origin, vec_scale(t, r.direction));
}

struct vec reflect(struct vec v, struct vec n) {
  return vec_sub(v, vec_scale(2*vec_dot(v,n), n));
}

struct aabb aabb_enclosing(const struct aabb *box0, const struct aabb *box1) {
  struct vec small = {
    fmin(box0->min.x, box1->min.x),
    fmin(box0->min.y, box1->min.y),
    fmin(box0->min.z, box1->min.z)
  };
  struct vec big = {
    fmax(box0->max.x, box1->max.x),
    fmax(box0->max.y, box1->max.y),
    fmax(box0->max.z, box1->max.z)
  };
  return (struct aabb){.min = small, .max = big};
}

struct vec aabb_centre(const struct aabb *a) {
  return (struct vec){a->min.x + (a->max.x - a->min.x),
                      a->min.y + (a->max.y - a->min.y),
                      a->min.z + (a->max.z - a->min.z)};
}
