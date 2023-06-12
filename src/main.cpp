#include "color.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "util.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
    // Image info
    const auto aspect_ratio = 16.0/9.0;
    const int image_width = 1080;
    const int image_height = static_cast<int>(image_width/aspect_ratio);
    // How many samples for every pixel
    const int samples_per_pixel = 100;

    // World
    HittableList world;
    world.add(std::make_shared<Sphere>(Point3(0,0,-1),0.5));
    world.add(std::make_shared<Sphere>(Point3(0,-100.5,-1),100));


    //Camera info
    Camera cam;

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for(int i = image_height-1;i>=0;i--){
        std::cerr << "\rScanlines remaining: \e[44m" << i << "\e[0m " << std::flush;
        for(int j = 0;j<image_width;j++){
            Color pixel_color(0,0,0);
            for(int k = 0;k<samples_per_pixel;k++){
                auto u = (((double)j+random_double())/(image_width-1));
                auto v = (((double)i+random_double())/(image_height-1));
                pixel_color += ray_color(cam.get_ray(u,v),world);
            }
            write_color(std::cout,pixel_color,samples_per_pixel);
        }
    }

    std::cerr << "\n\e[42m Done.\e[0m\n";
    return 0;
}
