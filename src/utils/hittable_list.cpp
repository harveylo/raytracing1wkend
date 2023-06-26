#include "hittable_list.hpp"
#include <memory>
#include "box.hpp"
#include "bvh.hpp"
#include "constant_medium.hpp"
#include "hittable.hpp"
#include "material.hpp"
#include "texture.hpp"
#include "util.hpp"
#include "vec3.hpp"

bool HittableList::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const{
    HitRecord temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;
    for(const auto& object:objects){
        // Only update the record when the object is closer than the previous one
        if(object -> hit(r,t_min, closest_so_far,temp_rec)){
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}



bool HittableList::bounding_box(double time0, double time1, AABB& output_box) const{
    if(objects.empty()) return false;
    
    AABB temp;
    bool first_box = true;

    for(const auto& object : objects){
        if(!object -> bounding_box(time0,time1,temp)) return false;
        output_box = first_box? temp : AABB::surrounding_box(output_box, temp);
        first_box = false;
    }

    return true;
}


//Scenes
HittableList random_scene(){
    HittableList world;

    auto ground_material = std::make_shared<Lambertian>(std::make_shared<CheckerTexture>(CheckerTexture(Color(0.2,0.3,0.1),Color(0.9,0.9,0.9))));
    world.add(std::make_shared<Sphere>(Point3(0,-1000,0),1000,ground_material));

    for(int i = -11; i<11;i++){
        for(int j = -11;j<11;j++){
            auto choose_mat = random_double();
            Point3 center(i+0.9*random_double(),0.2,j+0.9*random_double());

            if((center - Point3(4,0.2,0)).length()>0.9){
                std::shared_ptr<Material> sphere_material;

                if(choose_mat<0.7){
                    // Diffuse, 70 percent probability
                    auto albedo = Vec3::random()*Vec3::random();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    auto center2 = center+Vec3(0,random_double(0,0.5),0);
                    world.add(std::make_shared<MovingSphere>(center,center2,0.2,sphere_material,0,1));
                    continue;
                }else if(choose_mat<0.85){
                    // Metal, 15 percent probability
                    auto albedo = Vec3::random(0.5,1);
                    auto fuzz = random_double(0,0.25);

                    sphere_material = std::make_shared<Metal>(albedo,fuzz);
                }else{
                    // Glass, 15 percent probability
                    sphere_material = std::make_shared<Dielectric>(random_double(1.5,2));
                }

                world.add(std::make_shared<Sphere>(center,0.2,sphere_material));
            }
        }
    }
    auto material1 = std::make_shared<Dielectric>(1.5);
    world.add(std::make_shared<Sphere>(Point3(0,1,0),1.0,material1));

    auto material2 = std::make_shared<Lambertian>(Color(0.1529,0.4588,0.7137));
    world.add(std::make_shared<Sphere>(Point3(-4,1,0),1.0,material2));

    auto material3 = std::make_shared<Metal>(Color(0.7,0.6,0.5),0.0);
    world.add(std::make_shared<Sphere>(Point3(4,1,0),1.0,material3));

    return world;
}

HittableList two_spheres(){
    HittableList objects;
    auto checker = std::make_shared<CheckerTexture>(Color(0.2,0.3,0.1),Color(0.9,0.9,0.9));

    objects.add(std::make_shared<Sphere>(Point3(0,-10,0),10,std::make_shared<Lambertian>(checker)));
    objects.add(std::make_shared<Sphere>(Point3(0,10,0),10,std::make_shared<Lambertian>(checker)));
    return objects;
}


HittableList two_perlin_spheres(){
    HittableList objects;

    auto pertext = std::make_shared<NoiseTexture>(4);

    objects.add(std::make_shared<Sphere>(Point3(0,-1000,0),1000,std::make_shared<Lambertian>(pertext)));
    objects.add(std::make_shared<Sphere>(Point3(0,2,0),2,std::make_shared<Lambertian>(pertext)));

    return objects;
}

HittableList earth(){
    auto earth_texture = std::make_shared<ImageTexture>("resources/earthmap.jpg");
    auto earth_surface = std::make_shared<Lambertian>(earth_texture);
    auto globe = std::make_shared<Sphere>(Point3(0,0,0),2,earth_surface);

    return HittableList(globe);
}

HittableList simple_light(){
    HittableList objects;

    auto pretext = std::make_shared<NoiseTexture>(4);
    objects.add(std::make_shared<Sphere>(Point3(0,-1000,0),1000,std::make_shared<Lambertian>(pretext)));
    objects.add(std::make_shared<Sphere>(Point3(0,2,0),2,std::make_shared<Lambertian>(pretext)));

    auto difflight = std::make_shared<DiffuseLight>(Color(5,5,5));
    objects.add(std::make_shared<Sphere>(Point3(0,8,0),2,std::make_shared<DiffuseLight>(Color(10,10,10))));
    objects.add(std::make_shared<XYRect>(3,5,1,3,-2,difflight));

    return objects;
}

HittableList cornell_box(){
    HittableList objects;

    auto red = std::make_shared<Lambertian>(Color(.65,.05,.05));
    auto white = std::make_shared<Lambertian>(Color(.73,.73,.73));
    auto green = std::make_shared<Lambertian>(Color(.12,.45,.15));

    auto light = std::make_shared<DiffuseLight>(Color(15,15,15));

    objects.add(std::make_shared<YZRect>(0,555,0,555,555,green));
    objects.add(std::make_shared<YZRect>(0,555,0,555,0,red));
    objects.add(std::make_shared<XZRect>(213,343,227,332,554,light));
    objects.add(std::make_shared<XZRect>(0,555,0,555,0,white));
    objects.add(std::make_shared<XZRect>(0,555,0,555,555,white));
    objects.add(std::make_shared<XYRect>(0,555,0,555,555,white));
    
    std::shared_ptr<Hittable> box1 = std::make_shared<Box>(Point3(0,0,0),Point3(165,330,165),white);
    box1 = std::make_shared<RotateY>(box1,15);
    box1 = std::make_shared<Translate>(box1, Vec3(265,0,295));
    objects.add(box1);
    
    std::shared_ptr<Hittable> box2 = std::make_shared<Box>(Point3(0,0,0),Point3(165,165,165),white);
    box2 = std::make_shared<RotateY>(box2, -18);
    box2 = std::make_shared<Translate>(box2,Vec3(130,0,65));
    objects.add(box2);

    return objects;
}

HittableList cornell_smoke(){
    HittableList objects;

    auto red   = std::make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = std::make_shared<DiffuseLight>(Color(7, 7, 7));

    objects.add(std::make_shared<YZRect>(0, 555, 0, 555, 555, green));
    objects.add(std::make_shared<YZRect>(0, 555, 0, 555, 0, red));
    objects.add(std::make_shared<XZRect>(113, 443, 127, 432, 554, light));
    objects.add(std::make_shared<XZRect>(0, 555, 0, 555, 555, white));
    objects.add(std::make_shared<XZRect>(0, 555, 0, 555, 0, white));
    objects.add(std::make_shared<XYRect>(0, 555, 0, 555, 555, white));

    std::shared_ptr<Hittable> box1 = std::make_shared<Box>(Point3(0,0,0), Point3(165,330,165), white);
    box1 = std::make_shared<RotateY>(box1, 15);
    box1 = std::make_shared<Translate>(box1, Vec3(265,0,295));

    std::shared_ptr<Hittable> box2 = std::make_shared<Box>(Point3(0,0,0), Point3(165,165,165), white);
    box2 = std::make_shared<RotateY>(box2, -18);
    box2 = std::make_shared<Translate>(box2, Vec3(130,0,65));

    objects.add(std::make_shared<ConstantMedium>(box1, 0.01, Color(0,0,0)));
    objects.add(std::make_shared<ConstantMedium>(box2, 0.01, Color(1,1,1)));

    return objects;
}

HittableList final_scene(){
    HittableList boxes1;
    auto ground = std::make_shared<Lambertian>(Color(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i*w;
            auto z0 = -1000.0 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1,101);
            auto z1 = z0 + w;

            boxes1.add(std::make_shared<Box>(Point3(x0,y0,z0), Point3(x1,y1,z1), ground));
        }
    }

    HittableList objects;

    objects.add(std::make_shared<BVHNode>(boxes1, 0, 1));

    auto light = std::make_shared<DiffuseLight>(Color(7, 7, 7));
    objects.add(std::make_shared<XZRect>(123, 423, 147, 412, 554, light));

    auto center1 = Point3(400, 400, 200);
    auto center2 = center1 + Vec3(30,0,0);
    auto moving_sphere_material = std::make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
    objects.add(std::make_shared<MovingSphere>(center1, center2, 50, moving_sphere_material,0,1));

    objects.add(std::make_shared<Sphere>(Point3(260, 150, 45), 50, std::make_shared<Dielectric>(1.5)));
    objects.add(std::make_shared<Sphere>(
        Point3(0, 150, 145), 50, std::make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0)
    ));

    auto boundary = std::make_shared<Sphere>(Point3(360,150,145), 70, std::make_shared<Dielectric>(1.5));
    objects.add(boundary);
    objects.add(std::make_shared<ConstantMedium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
    boundary = std::make_shared<Sphere>(Point3(0, 0, 0), 5000, std::make_shared<Dielectric>(1.5));
    objects.add(std::make_shared<ConstantMedium>(boundary, .0001, Color(1,1,1)));

    auto emat = std::make_shared<Lambertian>(std::make_shared<ImageTexture>("resources/earthmap.jpg"));
    objects.add(std::make_shared<Sphere>(Point3(400,200,400), 100, emat));
    auto pertext = std::make_shared<NoiseTexture>(0.1);
    objects.add(std::make_shared<Sphere>(Point3(220,280,300), 80, std::make_shared<Lambertian>(pertext)));

    HittableList boxes2;
    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(std::make_shared<Sphere>(Point3::random(0,165), 10, white));
    }

    objects.add(std::make_shared<Translate>(
        std::make_shared<RotateY>(
            std::make_shared<BVHNode>(boxes2, 0.0, 1.0), 15),
            Vec3(-100,270,395)
        )
    );

    return objects;
}