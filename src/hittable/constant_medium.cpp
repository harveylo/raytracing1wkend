#include "constant_medium.hpp"
#include "hittable.hpp"
#include "util.hpp"
#include "vec3.hpp"


bool ConstantMedium::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    HitRecord rec0, rec1;

    // Check if the infinite length ray can hit the bounding box of the boundary
    if(!boundary->hit(r, -INFIN_D, INFIN_D, rec0)) return false;

    // If hit, check whether half infinite ray can hit the secend time
    if(!boundary->hit(r, rec0.t+0.0001, INFIN_D, rec1))
        return false;
    
    // Restrain the t
    if(rec0.t < t_min) rec0.t = t_min;
    if(rec1.t > t_max) rec1.t = t_max;

    // If t of the second hit is less than the first, return false
    if(rec1.t <= rec0.t) return false;

    // Restrain t of the first hit, make sure it is at the positive direction
    if(rec0.t < 0) rec0.t = 0;

    const auto ray_length = r.direction().length();
    const auto distance_inside_boundary = (rec1.t - rec0.t) * ray_length;
    const auto hit_distance = neg_inv_density * log(random_double());

    // If the travel distance is less than the random 'must-scatter' distance, pass through directly
    if(hit_distance > distance_inside_boundary) return false;

    rec.t = rec0.t + hit_distance / ray_length;
    rec.p = r.at(rec.t);

    rec.normal = Vec3::random_unit_vector();
    rec.front_face = true;
    rec.mat_ptr = phase_function;

    return true;


}