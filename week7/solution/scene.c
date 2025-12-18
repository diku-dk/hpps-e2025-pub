#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "geometry.h"
#include "random.h"
#include "scene.h"

struct material *object_material(struct object *o) {
  switch (o->type) {
  case SPHERE:
    return o->sphere.material;
  case XY_RECTANGLE:
    return o->xy_rectangle.material;
  case XZ_RECTANGLE:
    return o->xz_rectangle.material;
  case YZ_RECTANGLE:
    return o->yz_rectangle.material;
  default:
    abort();
  }
}

bool sphere_hit(struct sphere *s, struct ray *r,
                double t0, double t1, struct hit *hit) {
  struct vec oc = vec_sub(r->origin, s->centre);
  double a = vec_quadrance(r->direction);
  double b = vec_dot(oc, r->direction);
  double c = vec_quadrance(oc) - s->radius*s->radius;
  double discriminant = b*b - a*c;
  if (discriminant > 0) {
    double temp = (-b - sqrt(b*b-a*c))/a;
    if (temp < t1 && temp > t0) {
      hit->t = temp;
      hit->p = point_at_parameter(*r, temp);
      hit->normal = vec_scale(1/s->radius, vec_sub(point_at_parameter(*r, temp), s->centre));
      hit->material = s->material;
      return true;
    }
    temp = (-b + sqrt(b*b-a*c))/a;
    if (temp < t1 && temp > t0) {
      hit->t = temp;
      hit->p = point_at_parameter(*r, temp);
      hit->normal = vec_scale(1/s->radius, vec_sub(point_at_parameter(*r, temp), s->centre));
      hit->material = s->material;
      return true;
    }
  }
  return false;
}

bool xy_rectangle_hit(struct xy_rectangle *xy, struct ray *r,
                      double t0, double t1, struct hit *hit) {
  double t = (xy->k - r->origin.z) / r->direction.z;

  if (t < t0 || t > t1) {
    return false;
  }

  double x = r->origin.x + t*r->direction.x;
  double y = r->origin.y + t*r->direction.y;
  if (x < xy->x0 || x > xy->x1 || y < xy->y0 || y > xy->y1) {
    return false;
  }

  hit->p = point_at_parameter(*r, t);
  hit->normal.x = 0;
  hit->normal.y = 0;
  hit->normal.z = 1;
  hit->material = xy->material;
  hit->t = t;

  return true;
}

bool xz_rectangle_hit(struct xz_rectangle *xz, struct ray *r,
                      double t0, double t1, struct hit *hit) {
  double t = (xz->k - r->origin.y) / r->direction.y;

  if (t < t0 || t > t1) {
    return false;
  }

  double x = r->origin.x + t*r->direction.x;
  double z = r->origin.z + t*r->direction.z;
  if (x < xz->x0 || x > xz->x1 || z < xz->z0 || z > xz->z1) {
    return false;
  }

  hit->p = point_at_parameter(*r, t);
  hit->normal.x = 0;
  hit->normal.y = 1;
  hit->normal.z = 0;
  hit->material = xz->material;
  hit->t = t;

  return true;
}

bool yz_rectangle_hit(struct yz_rectangle *yz, struct ray *r,
                      double t0, double t1, struct hit *hit) {
  double t = (yz->k - r->origin.x) / r->direction.x;

  if (t < t0 || t > t1) {
    return false;
  }

  double y = r->origin.y + t*r->direction.y;
  double z = r->origin.z + t*r->direction.z;

  if (y < yz->y0 || y > yz->y1 || z < yz->z0 || z > yz->z1) {
    return false;
  }

  hit->p = point_at_parameter(*r, t);
  // Hack the surface normal based on the expectation that the camera is always
  // at x=0.
  hit->normal.x = yz->k > 0 ? -1 : 1;
  hit->normal.y = 0;
  hit->normal.z = 0;
  hit->material = yz->material;
  hit->t = t;

  return true;
}

bool object_hit(struct object *o, struct ray *r,
                double t0, double t1, struct hit *hit) {
  switch (o->type) {
  case SPHERE:
    return sphere_hit(&o->sphere, r, t0, t1, hit);
  case XY_RECTANGLE:
    return xy_rectangle_hit(&o->xy_rectangle, r, t0, t1, hit);
  case XZ_RECTANGLE:
    return xz_rectangle_hit(&o->xz_rectangle, r, t0, t1, hit);
  case YZ_RECTANGLE:
    return yz_rectangle_hit(&o->yz_rectangle, r, t0, t1, hit);
  default:
    abort();
  }
}

bool scattering_lambertian(struct rng *rng, struct ray *r, struct hit *h, struct scattering *hit) {
  (void)r;
  struct vec bounce = random_in_unit_sphere(rng);
  struct vec target = vec_add(vec_add(h->p, h->normal), bounce);
  hit->attenuation = h->material->lambertian.albedo;
  hit->scattered.origin = h->p;
  hit->scattered.direction = vec_sub(target, h->p);
  return true;
}

bool scattering_metal(struct rng *rng, struct ray *r, struct hit *h, struct scattering *hit) {
  struct vec reflected = reflect(vec_normalise(r->direction), h->normal);
  struct vec bounce = random_in_unit_sphere(rng);
  struct ray scattered =
    (struct ray){.origin = h->p,
                 .direction = vec_add(reflected,
                                       vec_scale(h->material->metal.fuzz,
                                                  bounce))
  };
  if (vec_dot(scattered.direction, h->normal) > 0) {
    hit->attenuation = h->material->metal.albedo;
    hit->scattered = scattered;
    return true;
  } else {
    return false;
  }
}

bool refract(struct vec v, struct vec n, double ni_over_nt, struct vec* hit) {
  struct vec uv = vec_normalise(v);
  double dt = vec_dot(uv, n);
  double discriminant = 1 - ni_over_nt*ni_over_nt*(1-dt*dt);
  if (discriminant > 0) {
    *hit = vec_sub(vec_scale(ni_over_nt, vec_sub(uv, vec_scale(dt,n))),
                    vec_scale(sqrt(discriminant),n));
    return true;
  } {
    return false;
  }
}

double schlick (double cosine, double ref_idx) {
  double r0 = (1-ref_idx) / (1+ref_idx);
  r0 *= r0;
  return r0 + (1-r0)*pow((1-cosine),5);
}

bool scattering_dielectric(struct rng *rng, struct ray *r, struct hit *h, struct scattering *hit) {
  double ref_idx = h->material->dielectric.ref_idx;
  struct vec reflected = reflect(r->direction, h->normal);
  struct vec attenuation = (struct vec) {.x = 1, .y = 1, .z = 1};
  struct vec hitward_normal;
  double ni_over_nt, cosine;
  if (vec_dot(r->direction, h->normal)) {
    hitward_normal = vec_neg(h->normal);
    ni_over_nt = ref_idx;
    cosine = ref_idx * vec_dot(r->direction, h->normal) / vec_norm(r->direction);
  } else {
    hitward_normal = h->normal;
    ni_over_nt = 1/ref_idx;
    cosine = -vec_dot(r->direction, h->normal) / vec_norm(r->direction);
  }
  struct vec refracted;
  if (refract(r->direction, hitward_normal, ni_over_nt, &refracted)) {
    double reflect_prob = schlick(cosine, ref_idx);
    double x = random_double(rng);
    struct vec direction = x < reflect_prob ? reflected : refracted;
    hit->attenuation = attenuation;
    hit->scattered.origin = h->p;
    hit->scattered.direction=direction;
  } else {
    hit->attenuation = attenuation;
    hit->scattered.origin = h->p;
    hit->scattered.direction=reflected;
  }
  return true;
}


bool scattering(struct rng *rng, struct ray *r, struct hit *h, struct scattering *hit) {
  switch (h->material->type) {
  case LAMBERTIAN:
    return scattering_lambertian(rng, r, h, hit);
  case METAL:
    return scattering_metal(rng, r, h, hit);
  case DIELECTRIC:
    return scattering_dielectric(rng, r, h, hit);
  default:
    abort();
  }
}

void describe_material(struct material* material) {
    switch (material->type) {
    case LAMBERTIAN:
      printf("lambertian(albedo=(%f,%f,%f))\n",
              material->lambertian.albedo.x,
              material->lambertian.albedo.y,
              material->lambertian.albedo.z);
      break;
    case METAL:
      printf("metal(albedo=(%f,%f,%f), fuzz=%f)\n",
              material->metal.albedo.x,
              material->metal.albedo.y,
              material->metal.albedo.z,
              material->metal.fuzz);
      break;
    case DIELECTRIC:
      printf("dielectric(ref_idx=%f)\n",
             material->dielectric.ref_idx);
      break;
    default:
      abort();
    }
}

void describe_object(struct object* object) {
    switch (object->type) {
    case SPHERE:
      printf("sphere\n  centre=(%f,%f,%f)\n  material=",
              object->sphere.centre.x,
              object->sphere.centre.y,
              object->sphere.centre.z);
      describe_material(object->sphere.material);
      break;
    case XY_RECTANGLE:
      printf("xy_rectangle\n  x0=%f\n  x1=%f\n  y0=%f\n  y1=%f\n  k=%f\n  material=",
              object->xy_rectangle.x0,
              object->xy_rectangle.x1,
              object->xy_rectangle.y0,
              object->xy_rectangle.y1,
              object->xy_rectangle.k);
      describe_material(object->xy_rectangle.material);
      break;
    case XZ_RECTANGLE:
      printf("xz_rectangle\n  x0=%f\n  x1=%f\n  z0=%f\n  z1=%f\n  k=%f\n  material=",
              object->xz_rectangle.x0,
              object->xz_rectangle.x1,
              object->xz_rectangle.z0,
              object->xz_rectangle.z1,
              object->xz_rectangle.k);
      describe_material(object->xz_rectangle.material);
      break;
    case YZ_RECTANGLE:
      printf("yz_rectangle\n  y0=%f\n  y1=%f\n  z0=%f\n  z1=%f\n  k=%f\n  material=",
              object->yz_rectangle.y0,
              object->yz_rectangle.y1,
              object->yz_rectangle.z0,
              object->yz_rectangle.z1,
              object->yz_rectangle.k);
      describe_material(object->yz_rectangle.material);
      break;
    default:
      abort();
    }
}

struct camera mk_camera(struct vec lookfrom, struct vec lookat,
                        double vfov, double aspect,
                        double aperture, double focus_dist) {
  struct vec vup = (struct vec){0,1,0};
  double theta = vfov * M_PI / 180;
  double half_height = tan (theta / 2);
  double half_width = aspect * half_height;
  struct vec w = vec_normalise(vec_sub(lookfrom,lookat));
  struct vec u = vec_normalise(vec_cross(vup,w));
  struct vec v = vec_cross(w,u);
  return (struct camera){
    .lower_left_corner =
    vec_sub(vec_sub(vec_sub(lookfrom,
                               vec_scale(half_width * focus_dist, u)),
                      vec_scale(half_height * focus_dist, v)),
             vec_scale(focus_dist, w)),
    .horizontal = vec_scale(2*half_width*focus_dist, u),
    .vertical = vec_scale(2*half_height*focus_dist, v),
    .origin = lookfrom,
    .u = u,
    .v = v,
    .w = w,
    .lens_radius = aperture / 2};

}

struct ray get_ray(struct rng *rng, struct camera* c, double s, double t) {
  struct vec p = random_in_unit_sphere(rng);
  struct vec rd = vec_scale(c->lens_radius, p);
  struct vec offset = vec_add(vec_scale(rd.x, c->u), vec_scale(rd.y, c->v));
  return (struct ray){
    .origin = vec_add(offset, c->origin),
    .direction =
    vec_sub(vec_sub(vec_add(vec_add(c->lower_left_corner,
                                        vec_scale(s, c->horizontal)),
                               vec_scale(t,c->vertical)),
                      c->origin),
             offset)
  };
}


void random_material(struct rng* rng, struct material* m) {
  switch (random_int(rng) % 3) {
  case 0:
    m->type = LAMBERTIAN;
    m->lambertian.albedo = random_vec(rng);
    break;
  case 1:
    m->type = METAL;
    m->metal.albedo = random_vec(rng);
    m->metal.fuzz = random_double(rng);
    break;
  case 2:
    m->type = DIELECTRIC;
    m->dielectric.ref_idx = random_double(rng)*2;
    break;
  default:
    abort();
  }
}

void random_object(struct rng* rng,
                   size_t num_materials, struct material *materials,
                   struct object* object) {
  switch (random_int(rng) % 4) {
  case SPHERE:
    object->type = SPHERE;
    object->sphere.centre.x = random_double(rng)*2-1 * 5;
    object->sphere.centre.y = random_double(rng)*-2/1 * 3;
    object->sphere.centre.z = random_double(rng)*2-1 * 5;
    object->sphere.radius = random_double(rng);
    object->sphere.material = &materials[random_int(rng)%num_materials];
    break;
  case XY_RECTANGLE:
    object->type = XY_RECTANGLE;
    object->xy_rectangle.x0 = random_double(rng)*2-1 * 5;
    object->xy_rectangle.x1 = object->xy_rectangle.x0 + random_double(rng) * 4;
    object->xy_rectangle.y0 = random_double(rng)*2-1;
    object->xy_rectangle.y1 = object->xy_rectangle.y0 + random_double(rng) * 4;
    object->xy_rectangle.k = random_double(rng)*2-1 * 5;
    object->xy_rectangle.material = &materials[random_int(rng)%num_materials];
    break;
  case XZ_RECTANGLE:
    object->type = XZ_RECTANGLE;
    object->xz_rectangle.x0 = random_double(rng)*2-1 * 5;
    object->xz_rectangle.x1 = object->xz_rectangle.x0 + random_double(rng) * 4;
    object->xz_rectangle.z0 = random_double(rng)*2-1;
    object->xz_rectangle.z1 = object->xz_rectangle.z0 + random_double(rng) * 4;
    object->xz_rectangle.k = random_double(rng)*2-1 * 5;
    object->xz_rectangle.material = &materials[random_int(rng)%num_materials];
    break;
  case YZ_RECTANGLE:
    object->type = YZ_RECTANGLE;
    object->yz_rectangle.y0 = random_double(rng)*2-1 * 5;
    object->yz_rectangle.y1 = object->yz_rectangle.y0 + random_double(rng) * 4;
    object->yz_rectangle.z0 = random_double(rng)*2-1;
    object->yz_rectangle.z1 = object->yz_rectangle.z0 + random_double(rng) * 4;
    object->yz_rectangle.k = random_double(rng)*2-1 * 5;
    object->yz_rectangle.material = &materials[random_int(rng)%num_materials];
    break;
  default:
    abort();
  }
}

void empty_scene(struct vec *lookfrom, struct vec *lookat,
                 size_t *num_materials, struct material **materials,
                 size_t *num_objects, struct object **objects) {
  *lookfrom = (struct vec){0,2,3};
  *lookat = (struct vec){0,0,0};
  *num_materials = 0;
  *num_objects = 0;
  *materials = NULL;
  *objects = NULL;
}

void random_scene(struct vec *lookfrom, struct vec *lookat,
                  size_t *num_materials, struct material **materials,
                  size_t *num_objects, struct object **objects) {
  struct rng rng;
  seed_rng(&rng, time(NULL));

  *lookfrom = (struct vec){0,2,3};
  *lookat = (struct vec){0,0,0};

  *num_materials = 10;
  *materials = calloc(*num_materials, sizeof(struct material));
  for (size_t i = 0; i < *num_materials; i++) {
    random_material(&rng, &(*materials)[i]);
  }

  *num_objects = 30;
  *objects = calloc(*num_objects, sizeof(struct object));

  for (size_t i = 0; i < *num_objects; i++) {
    random_object(&rng, *num_materials, *materials, &(*objects)[i]);
  }
}

void nice_scene(struct vec *lookfrom, struct vec *lookat,
                size_t *num_materials, struct material **materials_out,
                size_t *num_objects, struct object **objects_out) {
  *lookfrom = (struct vec){0,3,3};
  *lookat = (struct vec){0,0,0};

  *num_materials = 4;
  *num_objects = 4;

  struct material *materials = calloc(*num_materials, sizeof(struct material));
  materials[0].type = METAL;
  materials[0].metal.albedo = (struct vec) { 1, 0, 0 };
  materials[0].metal.fuzz = 0.9;
  materials[1].type = METAL;
  materials[1].metal.albedo = (struct vec) { 0, 1, 0 };
  materials[1].metal.fuzz = 0.9;
  materials[2].type = METAL;
  materials[2].metal.albedo = (struct vec) { 0, 0, 1 };
  materials[2].metal.fuzz = 0.9;
  materials[3].type = LAMBERTIAN;
  materials[3].lambertian.albedo = (struct vec) { 0.8, 0.8, 0.8 };

  struct object *objects = calloc(*num_objects, sizeof(struct object));

  objects[0].type = YZ_RECTANGLE;
  objects[0].yz_rectangle.material = &materials[0];
  objects[0].yz_rectangle.y0 = -5;
  objects[0].yz_rectangle.y1 = 0;
  objects[0].yz_rectangle.z0 = -5;
  objects[0].yz_rectangle.z1 = 5;
  objects[0].yz_rectangle.k = -4;

  objects[1].type = YZ_RECTANGLE;
  objects[1].yz_rectangle.material = &materials[1];
  objects[1].yz_rectangle.y0 = -5;
  objects[1].yz_rectangle.y1 = 0;
  objects[1].yz_rectangle.z0 = -5;
  objects[1].yz_rectangle.z1 = 5;
  objects[1].yz_rectangle.k = 4;

  objects[2].type = SPHERE;
  objects[2].sphere.material = &materials[3];
  objects[2].sphere.centre = (struct vec) { 4,0,-6 };
  objects[2].sphere.radius = 2;

  objects[3].type = XY_RECTANGLE;
  objects[3].xy_rectangle.x0 = -4;
  objects[3].xy_rectangle.x1 = 4;
  objects[3].xy_rectangle.y0 = -5;
  objects[3].xy_rectangle.y1 = 0;
  objects[3].xy_rectangle.k = -5;
  objects[3].xy_rectangle.material = &materials[2];

  *materials_out = materials;
  *objects_out = objects;
}

void irreg_scene(struct vec *lookfrom, struct vec *lookat,
                 size_t *num_materials, struct material **materials_out,
                 size_t *num_objects, struct object **objects_out) {
  *lookfrom = (struct vec){0, 12, 30};
  *lookat = (struct vec){0, 10, -1};
  int n = 50;
  double k = 600;
  double k2 = -k / 2;
  double kn = k / n;
  double kn2 = kn / 2;
  *num_materials = 1;
  struct material *material = malloc(sizeof(struct material));
  *materials_out = material;

  material->type = DIELECTRIC;
  material->dielectric.ref_idx = 1;

  *num_objects = n*n;
  struct object *objects = calloc(*num_objects, sizeof(struct object));

  int i = 0;
  for (int x = 0; x < n; x++) {
    for (int z = 0; z < n; z++) {
      objects[i].type = SPHERE;
      objects[i].sphere.material = material;
      objects[i].sphere.centre.x = k2+kn*x;
      objects[i].sphere.centre.y = 0;
      objects[i].sphere.centre.z = k2+kn*z;
      objects[i].sphere.radius = kn2;
      i++;
    }
  }
  *objects_out = objects;
}


void rgbbox_scene(struct vec *lookfrom, struct vec *lookat,
                  size_t *num_materials, struct material **materials_out,
                  size_t *num_objects, struct object **objects_out) {
  *lookfrom = (struct vec){0, 30, 30};
  *lookat = (struct vec){0, -1, -1};
  int n = 10;
  double k = 60;
  double k2 = -k / 2;
  double kn = k / n;
  double kn2 = kn / 2;
  *num_materials = 4;
  struct material *materials = calloc(*num_materials, sizeof(struct material));
  *materials_out = materials;

  materials[0].type = METAL;
  materials[0].metal.albedo = (struct vec){1,0,0};
  materials[0].metal.fuzz = 0.1;
  materials[1].type = METAL;
  materials[1].metal.albedo = (struct vec){0,1,0};
  materials[1].metal.fuzz = 0.1;
  materials[2].type = METAL;
  materials[2].metal.albedo = (struct vec){0,0,1};
  materials[2].metal.fuzz = 0.1;
  materials[3].type = DIELECTRIC;
  materials[3].dielectric.ref_idx = 0.9;

  *num_objects = 4*n*n;
  struct object *objects = calloc(*num_objects, sizeof(struct object));
  *objects_out = objects;

  size_t i = 0;

  // left wall
  for (int y = 0; y < n; y++) {
    for (int z = 0; z < n; z++) {
      objects[i].type = SPHERE;
      objects[i].sphere.material = &materials[0];
      objects[i].sphere.centre.x = k2;
      objects[i].sphere.centre.y = k2+kn*y;
      objects[i].sphere.centre.z = k2+kn*z;
      objects[i].sphere.radius = kn2;
      i++;
    }
  }

  // mid wall
  for (int x = 0; x < n; x++) {
    for (int y = 0; y < n; y++) {
      objects[i].type = SPHERE;
      objects[i].sphere.material = &materials[1];
      objects[i].sphere.centre.x = k2 + kn*x;
      objects[i].sphere.centre.y = k2 + kn*y;
      objects[i].sphere.centre.z = k2;
      objects[i].sphere.radius = kn2;
      i++;
    }
  }

  // right wall
  for (int y = 0; y < n; y++) {
    for (int z = 0; z < n; z++) {
      objects[i].type = SPHERE;
      objects[i].sphere.material = &materials[2];
      objects[i].sphere.centre.x = -k2;
      objects[i].sphere.centre.y = k2 + kn*y;
      objects[i].sphere.centre.z = k2 + kn*z;
      objects[i].sphere.radius = kn2;
      i++;
    }
  }

  // bottom
  for (int x = 0; x < n; x++) {
    for (int z = 0; z < n; z++) {
      objects[i].type = SPHERE;
      objects[i].sphere.material = &materials[3];
      objects[i].sphere.centre.x = k2 + kn*x;
      objects[i].sphere.centre.y = k2;
      objects[i].sphere.centre.z = k2 + kn*z;
      objects[i].sphere.radius = kn2;
      i++;
    }
  }
}

bool scene_by_name(const char *name,
                   struct vec *lookfrom, struct vec *lookat,
                   size_t *num_materials, struct material **materials,
                   size_t *num_objects, struct object **objects) {
  if (strcmp(name, "empty") == 0) {
    empty_scene(lookfrom, lookat,
                num_materials, materials,
                num_objects, objects);
    return true;
  } else if (strcmp(name, "nice") == 0) {
    nice_scene(lookfrom, lookat,
               num_materials, materials,
               num_objects, objects);
    return true;
  } else if (strcmp(name, "random") == 0) {
    random_scene(lookfrom, lookat,
                 num_materials, materials,
                 num_objects, objects);
    return true;
  } else if (strcmp(name, "irreg") == 0) {
    irreg_scene(lookfrom, lookat,
                num_materials, materials,
                num_objects, objects);
    return true;
  } else if (strcmp(name, "rgbbox") == 0) {
    rgbbox_scene(lookfrom, lookat,
                 num_materials, materials,
                 num_objects, objects);
    return true;
  } else {
    return false;
  }
}

uint32_t encode_rgb(struct vec vec) {
  return ((uint32_t)(sqrt(vec.x)*255) << 16)
    | ((uint32_t)(sqrt(vec.y)*255) << 8)
    | ((uint32_t)(sqrt(vec.z)*255) << 0);
}

