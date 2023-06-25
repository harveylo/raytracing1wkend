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

void scene_init(int scene, BVHNode& world, Camera& cam, Color& background, const double aspect_ratio);


int main(int argc, char *argv[])
{
    // Initialize random seed
    srand(time(NULL));

    // Handle cmdline arguments
    cmdline::parser p;
    p.add<int>("scene", 's', "scene to render", false,0,cmdline::oneof<int>(0,1,2,3,4,5));
    p.add<int>("pixel_samples", 'p',"the number of sampling per pixel", false,100);
    p.add<int>("width",'w',"width of the output image", false,800);
    p.add<int>("batch",'b',"number of threads to be created to run, maximum: 1000",false,8); 
    p.add<double>("aspect_ratio",'r',"The aspect ratio in double", false,16.0/9.0);
    p.parse_check(argc,argv);

    const int BATCH = p.get<int>("batch")<=MAX_BATCH?p.get<int>("batch"):MAX_BATCH;

    // Image info
    const auto aspect_ratio = p.get<double>("aspect_ratio");
    const int image_width = p.get<int>("width");
    const int image_height = static_cast<int>(image_width/aspect_ratio);
    // How many samples for every pixel
    const int samples_per_pixel = p.get<int>("pixel_samples");
    // How many times of reflection each ray can make
    const int max_depth = 50;
    std::vector<std::vector<std::vector<int>>> image(image_height,std::vector<std::vector<int>>(image_width,std::vector<int>(3,0)));

    // Scene information   
    BVHNode world_node ;
    Camera cam;
    Color background;

    int scene_no = p.get<int>("scene");
    scene_init(scene_no, world_node, cam, background,aspect_ratio);

    auto time_begin = time(NULL);
    // Render
    std::counting_semaphore<MAX_BATCH> sem(BATCH);
    for(int i = image_height-1;i>=0;i--){
        sem.acquire();
        std::thread(
            scanline_render,i,std::ref(cam),std::ref(world_node),
            image_width,image_height,samples_per_pixel,max_depth,
            std::ref(background),std::ref(image),std::ref(sem)
        ).detach();
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
    std::cerr << "\n\e[42m Done.\e[0m Time used: "<<(time(NULL)-time_begin)<<" seconds\n";
    return 0;
}

void scene_init(int scene, BVHNode& world, Camera& cam, Color& background, const double aspect_ratio){
    Point3 lookfrom;
    Point3 lookat;
    auto vfov = 40;
    auto aperture = 0.0;
    auto dist_to_focus = 10;
    Vec3 vup(0,1,0);
    switch (scene) {
        default:
        case 0:
            // Cover 1: random balls
            world = BVHNode(random_scene(),0,1);
            background = Color(0.7,0.8,1.00);
            lookfrom = Point3(13,2,3);
            lookat = Point3(0,0,0);
            vfov = 20.0;
            aperture = 0.1;
            break;
        case 1:
            // Two Chcker texture spheres
            world = BVHNode(two_spheres(),0,1);
            background = Color(0.7,0.8,1.00);
            lookfrom = Point3(13,2,3);
            lookat = Point3(0,0,0);
            vfov = 20.0;
            break;
        case 2:
            // Perlin Spheres
            world = BVHNode(two_perlin_spheres(),0,1);
            background = Color(0.7,0.8,1.00);
            lookfrom = Point3(13,2,3);
            lookat = Point3(0,0,0);
            vfov = 20.0;
            break;
        case 3:
            // Earth
            world = BVHNode(earth(),0,1);
            background = Color(0.7,0.8,1.00);
            lookfrom = Point3(13,2,3);
            lookat = Point3(0,0,0);
            vfov = 20.0;
            break;
        case 4:
            // simple light
            world = BVHNode(simple_light(),0,1);
            background = Color(0.0,0.0,0.0);
            lookfrom = Point3(26,3,6);
            lookat = Point3(0,2,0);
            vfov = 20.0;
            break;
        case 5:
            // Cornell Box
            world = BVHNode(cornell_box(),0,1);
            background = Color(0,0,0);
            lookfrom = Point3(278,278,-800);
            lookat = Point3(278,278,0);
            vfov = 40.0;
            break;
    }
    cam = Camera(lookfrom,lookat,vup,vfov,aspect_ratio,aperture,dist_to_focus,0.0,1.0);
}