#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "geometry.h"
#include "random.h"

enum OBJECT_TYPE { SPHERE, XY_RECTANGLE, XZ_RECTANGLE, YZ_RECTANGLE };
enum MATERIAL_TYPE { LAMBERTIAN, METAL, DIELECTRIC };

struct lambertian {
  struct vec albedo;
};

struct metal {
  struct vec albedo;
  double fuzz;
};

struct dielectric {
  double ref_idx;
};

struct material {
  enum MATERIAL_TYPE type;
  union {
    struct lambertian lambertian;
    struct dielectric dielectric;
    struct metal metal;
  };
};

struct sphere {
  struct vec centre;
  double radius;
  struct material* material;
};

struct xy_rectangle {
  double x0, x1, y0, y1, k;
  struct material* material;
};

struct xz_rectangle {
  double x0, x1, z0, z1, k;
  struct material* material;
};


struct yz_rectangle {
  double y0, y1, z0, z1, k;
  struct material* material;
};

struct object {
  enum OBJECT_TYPE type;

  union {
    struct sphere sphere;
    struct xy_rectangle xy_rectangle;
    struct xz_rectangle xz_rectangle;
    struct yz_rectangle yz_rectangle;
  };
};

// Write textual representation of material to given file (which may be stdout).
void describe_material(struct material*);

// Write textual representation of object to given file (which may be stdout).
void describe_object(struct object*);

// A record determining the hit between a ray and an object.
struct hit {
  double t; // Distance until hit.
  struct vec p; // Point of hit.
  struct vec normal; // Surface normal at hit.
  struct material *material; // Material of object.
};

// How light scatters from the material.
struct scattering {
  struct vec attenuation;
  struct ray scattered;
};

// Retrieve the material of an object.
struct material *object_material(struct object *o);

// Find the intersection (if any) between the given object and a ray. The t0/t1
// arguments determine a minimum and maximum distance respectively - collisions
// outside this range are ignored. Store the result of the hit in the 'hit'
// argument. Returns true if a hit is determined.
bool object_hit(struct object *o, struct ray *r,
                double t0, double t1, struct hit *hit);

// Determine whether light scatters from this hit.
bool scattering(struct rng *rng, struct ray *r, struct hit *h, struct scattering *out);

// Camera parameters. Do not concern yourself with the details.
struct camera {
  struct vec origin;
  struct vec lower_left_corner;
  struct vec horizontal;
  struct vec vertical;
  struct vec u, v, w;
  double lens_radius;
};

// Get a ray emitted from a camera.
struct ray get_ray(struct rng *rng, struct camera* c, double s, double t);

// Construct a camera located at some point ('lookfrom') and looking at some
// other point ('lookat'). The 'vfov', 'aspect', 'aperture', and 'focus_dist'
// arguments determine optical properties of the camera, but are not important
// for us.
struct camera mk_camera(struct vec lookfrom, struct vec lookat,
                        double vfov, double aspect,
                        double aperture, double focus_dist);

// Encode (r,g,b) vector as RGB integer (using only three least significant
// bytes).
uint32_t encode_rgb(struct vec vec);
