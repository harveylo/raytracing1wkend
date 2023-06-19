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


bool Sphere::bounding_box(double time0, double time1, AABB& output_box) const{
    output_box = AABB(
        center - Vec3(radius,radius,radius),
        center + Vec3(radius,radius,radius)
    );
    return true;
}

bool MovingSphere::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const{
    double cur_time = r.time;
    Point3 cur_center = this->center(cur_time);
    Vec3 a_c = r.origin() - cur_center;
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
    rec.set_face_normal(r, (rec.p-cur_center)/radius);
    rec.set_material(mat_ptr);
    return true;
}


bool MovingSphere::bounding_box(double time0, double time1, AABB& output_box) const{
    Point3 pre = center(time0);
    Point3 post = center(time1);

    AABB a(
        pre - Vec3(radius,radius,radius),
        pre + Vec3(radius,radius,radius)
    );
    AABB b(
        post - Vec3(radius,radius,radius),
        post + Vec3(radius, radius,radius)
    );
    output_box=AABB::surrounding_box(a,b);
    return true;
}