#include "color.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "util.hpp"
#include "hittable_list.hpp"
#include "camera.hpp"
#include <iostream>
#include "material.hpp"
#include <memory>


int main(int argc, char const *argv[])
{
    // Image info
    const auto aspect_ratio = 16.0/9.0;
    const int image_width = 720;
    const int image_height = static_cast<int>(image_width/aspect_ratio);
    // How many samples for every pixel
    const int samples_per_pixel = 100;
    // How many times of reflection each ray can make
    const int max_depth = 50;

    // World
    auto world = random_scene();

    //Camera info
    Point3 lookfrom(13,2,3);
    Point3 lookat(0,0,0);
    Vec3 vup(0,1,0);
    auto dist_to_focus = 10;
    auto aperture = 0.1;
    Camera cam(lookfrom,lookat,vup,20, aspect_ratio,aperture,dist_to_focus);

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for(int i = image_height-1;i>=0;i--){
        std::cerr << "\rScanlines remaining: \e[44m" << i << "\e[0m " << std::flush;
        for(int j = 0;j<image_width;j++){
            Color pixel_color(0,0,0);
            for(int k = 0;k<samples_per_pixel;k++){
                auto u = (((double)j+random_double())/(image_width-1));
                auto v = (((double)i+random_double())/(image_height-1));
                pixel_color += ray_color(cam.get_ray(u,v),world, max_depth);
            }
            write_color(std::cout,pixel_color,samples_per_pixel);
        }
    }

    std::cerr << "\n\e[42m Done.\e[0m\n";
    return 0;
}
