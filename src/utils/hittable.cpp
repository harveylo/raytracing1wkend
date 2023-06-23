#include "cmath"
#include "hittable.hpp"
#include "util.hpp"
#include <math.h>



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
    auto outward_normal = (rec.p-center)/radius;
    // Whether radius is negtive or positive does not affect the above calculation
    // But if the radius is negtive the following calculation will lead to a reversed normal
    rec.set_face_normal(r, (rec.p-center)/radius);
    get_sphere_uv(outward_normal, rec.u, rec.v);
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


void Sphere::get_sphere_uv(const Point3& p, double& u, double& v){
    // p is the point on the unit sphere
    // u: returned value [0,1] of angle around the Y axis from X=-1.
    // v: returned value [0,1] of angle from Y=-1 to Y=+1.
    //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
    //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
    //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>


    auto phi = atan2(-p.z(), p.x()) + PI;
    auto theta = acos(-p.y());

    u = phi/(2*PI);
    v = theta/PI;
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