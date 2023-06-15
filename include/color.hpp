#ifndef __COLOR_H__
#define __COLOR_H__

#include "vec3.hpp"
#include "ray.hpp"
#include "hittable_list.hpp"
#include <iostream>
#include <vector>
#include <semaphore>
#include "camera.hpp"

void write_color(std::ostream &out, Color pixel_color, int samples_per_pixel);
Color ray_color(const Ray& r, const HittableList& world, int depth);

void scanline_render(const int line,
                     const Camera& cam,
                     const HittableList& world,
                     const int image_width,
                     const int image_height,
                     const int samples_per_pixel,
                     const int max_depth,
                     std::vector<std::vector<std::vector<int>>>& image,
                     std::counting_semaphore<BATCH>& sem);
#endif