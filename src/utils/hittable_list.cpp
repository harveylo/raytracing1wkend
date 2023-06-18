#include "hittable_list.hpp"
#include <memory>
#include "material.hpp"
#include "util.hpp"

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

HittableList random_scene(){
    HittableList world;

    auto ground_material = std::make_shared<Lambertian>(Color(0.5,0.5,0.5));
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