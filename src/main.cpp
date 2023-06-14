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
    HittableList world;
    auto material_ground = std::make_shared<Lambertian>(Color(0.8,0.8,0.0));
    auto material_center = std::make_shared<Lambertian>(Color(0.7,0.3,0.3));
    auto material_left = std::make_shared<Metal>(Color(0.8,0.8,0.8),0.3);
    auto material_right = std::make_shared<Metal>(Color(0.8,0.6,0.2),1.0);
    auto material_center_glass = std::make_shared<Dielectric> (1.5);
    auto material_left_glass = std::make_shared<Dielectric> (1.5);

    world.add(std::make_shared<Sphere>(Point3(0,0,-1),0.5,material_center));
    world.add(std::make_shared<Sphere>(Point3(0,-100.5,-1),100,material_ground));
    world.add(std::make_shared<Sphere>(Point3(-1.0,0.0,-1.0),0.5,material_left_glass));
    world.add(std::make_shared<Sphere>(Point3(-1.0,0.0,-1.0),-0.4,material_left_glass));
    world.add(std::make_shared<Sphere>(Point3(1.0,0.0,-1.0),0.5,material_right));


    //Camera info
    Camera cam(Point3(-2,2,1),Point3(0,0,-1),Vec3(1,1,0),20, aspect_ratio);

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
