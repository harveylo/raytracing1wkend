#include "hittable_list.hpp"
#include <memory>
#include "box.hpp"
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