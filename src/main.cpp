#include "3rdparty/cmdline.hpp"
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
#include <string>

void scene_init(int scene, BVHNode& world, Camera& cam, const double aspect_ratio);


int main(int argc, char *argv[])
{
    // Initialize random seed
    srand(time(NULL));

    // Handle cmdline arguments
    cmdline::parser p;
    p.add<int>("scene", 's', "scene to render", false,0,cmdline::oneof<int>(0,1,2));
    p.add<int>("sample_times", 't',"the time of sampling", false,100);
    p.add<int>("width",'w',"width of the output image", false,720);
    p.parse_check(argc,argv);

    // Image info
    const auto aspect_ratio = 16.0/9.0;
    const int image_width = p.get<int>("width");
    const int image_height = static_cast<int>(image_width/aspect_ratio);
    // How many samples for every pixel
    const int samples_per_pixel = p.get<int>("sample_times");
    // How many times of reflection each ray can make
    const int max_depth = 50;
    std::vector<std::vector<std::vector<int>>> image(image_height,std::vector<std::vector<int>>(image_width,std::vector<int>(3,0)));
    
    BVHNode world_node ;

    Camera cam;

    int scene_no = p.get<int>("scene");
    scene_init(scene_no, world_node, cam, aspect_ratio);

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

void scene_init(int scene, BVHNode& world, Camera& cam, const double aspect_ratio){
    Point3 lookfrom;
    Point3 lookat;
    auto vfov = 40;
    auto aperture = 0.0;
    auto dist_to_focus = 10;
    Vec3 vup(0,1,0);
    switch (scene) {
        default:
        case 0:
            world = BVHNode(random_scene(),0,1);
            lookfrom = Point3(13,2,3);
            lookat = Point3(0,0,0);
            vfov = 20.0;
            aperture = 0.1;
            break;
        case 1:
            world = BVHNode(two_spheres(),0,1);
            lookfrom = Point3(13,2,3);
            lookat = Point3(0,0,0);
            vfov = 20.0;
            break;
        case 2:
            world = BVHNode(two_perlin_spheres(),0,1);
            lookfrom = Point3(13,2,3);
            lookat = Point3(0,0,0);
            vfov = 20.0;
            break;
    }
    cam = Camera(lookfrom,lookat,vup,vfov,aspect_ratio,aperture,dist_to_focus,0.0,1.0);
}