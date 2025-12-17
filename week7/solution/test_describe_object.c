#include "scene.h"

int main(void) {
  size_t num_materials = 4;
  size_t num_objects = 4;

  struct material *materials = calloc(num_materials, sizeof(struct material));
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

  struct object *objects = calloc(num_objects, sizeof(struct object));

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

  for (size_t i = 0; i < num_objects; i++) {
    describe_object(&objects[i]);
  }

}
