#include "color.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
    // Image info
    const auto aspect_ratio = 16.0/9.0;
    const int image_width = 1080;
    const int image_height = static_cast<int>(image_width/aspect_ratio);


    //Camera info
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    //origin point of the coordinate system
    auto origin = Point3(0,0,0);
    //horizontal and vertical vectors
    auto horizontal = Vec3(viewport_width, 0, 0);
    auto vertical = Vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - Vec3(0,0,focal_length);
    std::cerr<<lower_left_corner<<std::endl;


    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for(int i = image_height-1;i>=0;i--){
        std::cerr << "\rScanlines remaining: \e[44m" << i << "\e[0m " << std::flush;
        for(int j = 0;j<image_width;j++){
            auto u = (double) j / (image_width-1);
            auto v = (double) i / (image_height-1);
            Ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            Color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\n\e[42m Done.\e[0m\n";
    return 0;
}
