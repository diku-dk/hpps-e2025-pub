#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "geometry.h"

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

struct scene {
  size_t num_materials;
  struct material *materials;
  size_t num_objects;
  struct object *objects;
};

struct hit {
  double t;
  struct vec p;
  struct vec normal;
  struct material *material;
};

struct scattering {
  struct vec attenuation;
  struct ray scattered;
};

// Retrieve the material of an object.
struct material *object_material(struct object *o);

bool sphere_hit(struct sphere *s, struct ray *r,
                double t0, double t1, struct hit *out);

bool xy_rectangle_hit(struct xy_rectangle *xy, struct ray *r,
                      double t0, double t1, struct hit *out);

bool xz_rectangle_hit(struct xz_rectangle *xz, struct ray *r,
                      double t0, double t1, struct hit *out);

bool yz_rectangle_hit(struct yz_rectangle *yz, struct ray *r,
                      double t0, double t1, struct hit *out);

bool object_hit(struct object *o, struct ray *r,
                double t0, double t1, struct hit *out);

bool scattering_lambertian(struct ray *r, struct hit *h, struct scattering *out);

bool scattering_metal(struct ray *r, struct hit *h, struct scattering *out);

bool scattering_dielectric(struct ray *r, struct hit *h, struct scattering *out);

bool scattering(struct ray *r, struct hit *h, struct scattering *out);

// Write textual representation of material to given file (which may be stdout).
void describe_material(struct material*);

// Write textual representation of object to given file (which may be stdout).
void describe_object(struct object*);

struct camera {
  struct vec origin;
  struct vec lower_left_corner;
  struct vec horizontal;
  struct vec vertical;
  struct vec u, v, w;
  double lens_radius;
};

struct ray get_ray(struct camera* c, double s, double t);

struct camera mk_camera(struct vec lookfrom, struct vec lookat, struct vec vup,
                        double vfov, double aspect,
                        double aperture, double focus_dist);

// Encode (r,g,b) vector as RGB integer (using only three least significant
// bytes).
uint32_t encode_rgb(struct vec vec);
