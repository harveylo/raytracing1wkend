#ifndef __COLOR_H__
#define __COLOR_H__

#include "bvh.hpp"
#include "util.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "hittable_list.hpp"
#include <iostream>
#include <vector>
#include <semaphore>
#include "camera.hpp"

void write_color(std::ostream &out, Color pixel_color, int samples_per_pixel);
Color ray_color(const Ray& r, const Color& background, const Hittable& world, int depth);

void scanline_render(const int line,
                     const Camera& cam,
                     const BVHNode& world,
                     const int image_width,
                     const int image_height,
                     const int samples_per_pixel,
                     const int max_depth,
                     const Color& background,
                     std::vector<std::vector<std::vector<int>>>& image,
                     std::counting_semaphore<MAX_BATCH>& sem);
#endif