#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "scene.h"

double random_double() {
  return rand()/((double)RAND_MAX);
}

bool find_hit(size_t num_objects, struct object *objects,
              struct ray *r,
              double t0, double t1, struct hit *hit) {
  double closest_so_far = t1;
  for (size_t i = 0; i < num_objects; i++) {
    if (object_hit(&objects[i], r, t0, closest_so_far, hit)) {
      closest_so_far = hit->t;
    }
  }
  return closest_so_far < t1;
}

struct vec colour(int max_depth,
                   size_t num_objects, struct object* objects,
                   struct ray *r) {
  if (max_depth <= 0) {
    return (struct vec){1,1,1};
  } else {
    struct hit hit;
    if (find_hit(num_objects, objects, r, 0.001, INFINITY, &hit)) {
      struct scattering s;
      if (scattering(r, &hit, &s)) {
        return
          vec_mul(s.attenuation,
                   colour(max_depth-1, num_objects, objects, &s.scattered));
      } else {
        return (struct vec){0,0,0};
      }
    } else {
      struct vec unit_direction = vec_normalise(r->direction);
      double t = 0.5 * (unit_direction.y + 1);
      return vec_add((struct vec){1-t,1-t,1-t}, (struct vec){t*0.5,t*0.7,t});
    }
  }
}

void render(int max_depth, int nx, int ny, int ns,
            size_t num_objects, struct object *objects,
            struct camera *camera,
            uint32_t *out) {
  for (int x = 0; x < nx; x++) {
    for (int y = 0; y < ny; y++) {
      struct vec pixel = {0,0,0};
      for (int iter = 0; iter < ns; iter++) {
        double ud = rand()/((double)RAND_MAX);
        double vd = rand()/((double)RAND_MAX);
        double u = (x + ud) / nx;
        double v = (y + vd) / ny;
        struct ray r = get_ray(camera, u, v);
        pixel = vec_add(pixel, colour(max_depth, num_objects, objects, &r));
      }
      out[(ny-y-1)*nx+x] = encode_rgb(vec_scale(1.0/ns,pixel));
    }
  }
}

void ppm_to_file(char *filename, uint32_t *pixels, int height, int width) {
  FILE *file = fopen(filename, "wb");
  assert(file != NULL);

  fprintf(file, "P6\n%d %d\n255\n", width, height);

  for (int i = 0; i < height*width; i++) {
    unsigned char pixel[3];
    pixel[0] = pixels[i]>>16;
    pixel[1] = pixels[i]>>8;
    pixel[2] = pixels[i]>>0;
    fwrite(pixel, 1, 3, file);
  }

  fclose(file);
}

void random_material(struct material* m) {
  switch (rand() % 3) {
  case 0:
    m->type = LAMBERTIAN;
    m->lambertian.albedo = random_vec();
    break;
  case 1:
    m->type = METAL;
    m->metal.albedo = random_vec();
    m->metal.fuzz = random_double();
    break;
  case 2:
    m->type = DIELECTRIC;
    m->dielectric.ref_idx = random_double()*2;
    break;
  default:
    abort();
  }
}

void random_object(size_t num_materials, struct material *materials,
                   struct object* object) {
  switch (rand() % 4) {
  case SPHERE:
    object->type = SPHERE;
    object->sphere.centre.x = random_double()*2-1 * 5;
    object->sphere.centre.y = random_double()*-2/1 * 3;
    object->sphere.centre.z = random_double()*2-1 * 5;
    object->sphere.radius = random_double();
    object->sphere.material = &materials[rand()%num_materials];
    break;
  case XY_RECTANGLE:
    object->type = XY_RECTANGLE;
    object->xy_rectangle.x0 = random_double()*2-1 * 5;
    object->xy_rectangle.x1 = object->xy_rectangle.x0 + random_double() * 4;
    object->xy_rectangle.y0 = random_double()*2-1;
    object->xy_rectangle.y1 = object->xy_rectangle.y0 + random_double() * 4;
    object->xy_rectangle.k = random_double()*2-1 * 5;
    object->xy_rectangle.material = &materials[rand()%num_materials];
    break;
  case XZ_RECTANGLE:
    object->type = XZ_RECTANGLE;
    object->xz_rectangle.x0 = random_double()*2-1 * 5;
    object->xz_rectangle.x1 = object->xz_rectangle.x0 + random_double() * 4;
    object->xz_rectangle.z0 = random_double()*2-1;
    object->xz_rectangle.z1 = object->xz_rectangle.z0 + random_double() * 4;
    object->xz_rectangle.k = random_double()*2-1 * 5;
    object->xz_rectangle.material = &materials[rand()%num_materials];
    break;
  case YZ_RECTANGLE:
    object->type = YZ_RECTANGLE;
    object->yz_rectangle.y0 = random_double()*2-1 * 5;
    object->yz_rectangle.y1 = object->yz_rectangle.y0 + random_double() * 4;
    object->yz_rectangle.z0 = random_double()*2-1;
    object->yz_rectangle.z1 = object->yz_rectangle.z0 + random_double() * 4;
    object->yz_rectangle.k = random_double()*2-1 * 5;
    object->yz_rectangle.material = &materials[rand()%num_materials];
    break;
  default:
    abort();
  }
}

void empty_scene(size_t *num_materials, struct material **materials,
                 size_t *num_objects, struct object **objects) {
  *num_materials = 0;
  *num_objects = 0;
  *materials = NULL;
  *objects = NULL;
}

void random_scene(struct vec *lookfrom, struct vec *lookat,
                  size_t *num_materials, struct material **materials,
                  size_t *num_objects, struct object **objects) {
  *lookfrom = (struct vec){0,2,3};
  *lookat = (struct vec){0,0,0};

  *num_materials = 10;
  *materials = calloc(*num_materials, sizeof(struct material));
  for (size_t i = 0; i < *num_materials; i++) {
    random_material(&(*materials)[i]);
  }

  *num_objects = 30;
  *objects = calloc(*num_objects, sizeof(struct object));

  for (size_t i = 0; i < *num_objects; i++) {
    random_object(*num_materials, *materials, &(*objects)[i]);
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

int main(void) {
  srand(time(NULL));
  struct vec lookfrom, lookat;
  double dist_to_focus = 10;
  double aperture = 0.01;
  double fov = 75;
  int nx = 400, ny = 200;
  int ns = 10;
  int max_depth = 3;

  size_t num_materials, num_objects;
  struct material *materials;
  struct object *objects;
  nice_scene(&lookfrom, &lookat,
             &num_materials, &materials,
             &num_objects, &objects);

  for (size_t i = 0; i < num_objects; i++) {
    describe_object(&objects[i]);
  }

  struct camera cam = mk_camera(lookfrom, lookat, (struct vec){0,1,0}, fov,
                                (double)nx/(double)ny,
                                aperture, dist_to_focus);

  uint32_t *argbs = calloc(nx*ny, sizeof(uint32_t));

  render(max_depth, nx, ny, ns, num_objects, objects, &cam, argbs);

  ppm_to_file("out.ppm", argbs, ny, nx);

  free(argbs);
  free(materials);
  free(objects);

}
