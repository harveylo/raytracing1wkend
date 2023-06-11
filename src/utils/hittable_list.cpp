#include "hittable_list.hpp"

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