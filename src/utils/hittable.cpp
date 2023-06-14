#include "cmath"
#include "hittable.hpp"



bool Sphere::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const{
    Vec3 a_c = r.origin() - center;
    auto a = r.direction().length_squared();
    auto h = r.direction().dot(a_c);
    auto c = a_c.dot(a_c) - radius*radius;
    auto discriminant = h*h - a*c;
    if(discriminant<0) return false;
    auto root = sqrt(discriminant);    
    // Find the nearest root that lies in the acceptable range.
    auto t = (-h - root)/a;
    if(t>t_max || t<t_min){
        t = (-h + root)/a;
        if(t>t_max || t<t_min) return false;
    }
    rec.set_record(r.at(t), t);
    // Whether radius is negtive or positive does not affect the above calculation
    // But if the radius is negtive the following calculation will lead to a reversed normal
    rec.set_face_normal(r, (rec.p-center)/radius);
    rec.set_material(mat_ptr);
    return true;
}
