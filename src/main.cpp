#include "color.hpp"
#include "vec3.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
    // Image height and width
    const int image_width = 256;
    const int image_height = 256;

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for(int i = image_height-1;i>=0;i--){
        std::cerr << "\rScanlines remaining: \e[44m" << i << "\e[0m " << std::flush;
        for(int j = 0;j<image_width;j++){
            Color pixel_color((double)i/(image_height-1), (double)j/(image_width-1), 0.25);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\n\e[42m Done.\e[0m\n";
    return 0;
}
