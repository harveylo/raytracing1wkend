#include "bvh.hpp"
#include "color.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "util.hpp"
#include "hittable_list.hpp"
#include "camera.hpp"
#include <iostream>
#include "material.hpp"
#include <memory>
#include <vector>
#include <semaphore>
#include <thread>


int main(int argc, char const *argv[])
{
    // Initialize random seed
    srand(time(NULL));


    // Image info
    const auto aspect_ratio = 16.0/9.0;
    const int image_width = 720;
    const int image_height = static_cast<int>(image_width/aspect_ratio);
    // How many samples for every pixel
    const int samples_per_pixel = 1000;
    // How many times of reflection each ray can make
    const int max_depth = 50;
    std::vector<std::vector<std::vector<int>>> image(image_height,std::vector<std::vector<int>>(image_width,std::vector<int>(3,0)));
    

    // World
    auto world = random_scene();
    auto world_node = BVHNode(world,0,1);

    //Camera info
    Point3 lookfrom(13,2,3);
    Point3 lookat(0,0,0);
    Vec3 vup(0,1,0);
    auto dist_to_focus = 10;
    auto aperture = 0.1;
    Camera cam(lookfrom,lookat,vup,20, aspect_ratio,aperture,dist_to_focus,0,1);

    // Render
    std::counting_semaphore<BATCH> sem(BATCH);
    for(int i = image_height-1;i>=0;i--){
        sem.acquire();
        std::thread(scanline_render,i,std::ref(cam),std::ref(world_node),image_width,image_height,samples_per_pixel,max_depth,std::ref(image),std::ref(sem)).detach();
        std::cerr << "\rScanline: \e[44m" << i << "\e[0m " <<"submitted"<< std::flush;
    }

    // Output image
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for(int i = image_height-1;i>=BATCH;i--){
        for(int j = 0;j<image_width;j++){
            std::cout << image[i][j][0] << ' ' << image[i][j][1] << ' ' << image[i][j][2] << '\n';
        }
    }
    int count = 0;
    while(count<BATCH){
        sem.acquire();
        count++;
    }
    for(int i = BATCH-1;i>=0;i--){
        for(int j = 0;j<image_width;j++){
            std::cout << image[i][j][0] << ' ' << image[i][j][1] << ' ' << image[i][j][2] << '\n';
        }
    }
    std::cerr << "\n\e[42m Done.\e[0m\n";
    return 0;
}
