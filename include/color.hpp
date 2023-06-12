#ifndef __COLOR_H__
#define __COLOR_H__

#include "vec3.hpp"
#include "ray.hpp"
#include "hittable_list.hpp"
#include <iostream>

void write_color(std::ostream &out, Color pixel_color, int samples_per_pixel);
Color ray_color(const Ray& r, const HittableList& world, int depth);

#endif