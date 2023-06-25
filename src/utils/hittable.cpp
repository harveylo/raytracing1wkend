#include "cmath"
#include "hittable.hpp"
#include "ray.hpp"
#include "util.hpp"
#include "vec3.hpp"
#include <cmath>
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


bool XYRect::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const{
    auto t = (k-r.origin().z())/r.direction().z();
    if(t<t_min||t>t_max) return false;

    Point3 hit_point = r.at(t);

    if(!(hit_point.x()>=x0&&hit_point.x()<=x1&&hit_point.y()>=y0&&hit_point.y()<=y1)) return false;

    rec.p = hit_point;
    rec.u = (hit_point.x()-x0)/(x1-x0);
    rec.v = (hit_point.y()-y0)/(y1-y0);
    rec.t = t;

    auto out_normal= orientation;
    rec.set_face_normal(r, out_normal);

    rec.mat_ptr = mat_ptr;

    return true;
}

bool XZRect::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const{
    auto t = (k-r.origin().y())/r.direction().y();
    if(t<t_min||t>t_max) return false;

    Point3 hit_point = r.at(t);

    if(!(hit_point.x()>=x0&&hit_point.x()<=x1&&hit_point.z()>=z0&&hit_point.z()<=z1)) return false;

    rec.p = hit_point;
    rec.u = (hit_point.x()-x0)/(x1-x0);
    rec.v = (hit_point.z()-z0)/(z1-z0);
    rec.t = t;

    auto out_normal= orientation;
    rec.set_face_normal(r, out_normal);

    rec.mat_ptr = mat_ptr;

    return true;
}

bool YZRect::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const{
    auto t = (k-r.origin().x())/r.direction().x();
    if(t<t_min||t>t_max) return false;

    Point3 hit_point = r.at(t);

    if(!(hit_point.z()>=z0&&hit_point.z()<=z1&&hit_point.y()>=y0&&hit_point.y()<=y1)) return false;

    rec.p = hit_point;
    rec.u = (hit_point.y()-y0)/(y1-y0);
    rec.v = (hit_point.z()-z0)/(z1-z0);
    rec.t = t;

    auto out_normal= orientation;
    rec.set_face_normal(r, out_normal);

    rec.mat_ptr = mat_ptr;

    return true;
}


bool Translate::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const{
    Ray moved_r(r.origin() - offset, r.direction(),r.time);
    if(!ptr->hit(moved_r,t_min,t_max,rec)) return false;
    
    rec.p += offset;
    // ! I think the code next line is redundant
    // rec.set_face_normal(moved_r, rec.normal);
     return true;
}
bool Translate::bounding_box(double time0, double time1, AABB& output_box) const{
    AABB temp;
    if(!(ptr->bounding_box(time0, time1, temp))) return false;

    output_box = AABB(temp.min()+offset, temp.max()+offset);
    return true;
}



RotateY::RotateY(std::shared_ptr<Hittable> p, double angle):ptr(p){
    // Calculate radians;
    auto radians = degrees_to_radians(angle);

    sin_theta = sin(radians);
    cos_theta = cos(radians);
    // Get the bounding box of the original instance
    // The time is fixed, may be improved
    hasbox = ptr->bounding_box(0, 1, bbox);

    Point3 min(INFIN_D,INFIN_D,INFIN_D);
    Point3 max(-INFIN_D,-INFIN_D,-INFIN_D);

    // this triple loop will rotate each vertex of the bounding box
    for(int i = 0;i<2;i++){
        for(int j = 0;j<2;j++){
            for(int k = 0;k<2;k++){
                // Get each vertex
                auto x = i*bbox.max().x() + (1-i)*bbox.min().x();
                auto y = j*bbox.max().y() + (1-j)*bbox.min().y();
                auto z = k*bbox.max().z() + (1-k)*bbox.min().z();

                // Rotate the vertex
                auto newx = cos_theta*x + sin_theta*z;
                auto newz = cos_theta*z - sin_theta*x;

                Vec3 temp(newx,y,newz);
                for(int l = 0;l<3;l++){
                    min[l] = fmin(min[l],temp[l]);
                    max[l] = fmax(max[l],temp[l]);
                }
            }
        }
    }

    bbox = AABB(min,max);

}
bool RotateY::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const{
    auto origin = r.origin();
    auto direction = r.direction();

    origin[0] = cos_theta * r.origin()[0] -  sin_theta*r.origin()[2];
    origin[2] = sin_theta *r.origin()[0] + cos_theta *r.origin()[2];

    direction[0] = cos_theta * r. direction()[0] - sin_theta*r.direction()[2];
    direction[2] = sin_theta*r.direction()[0] + cos_theta * r.direction()[2];

    Ray rotated_r(origin,direction,r.get_time());

    if(!ptr -> hit(rotated_r, t_min, t_max, rec))
        return false;
    auto p = rec.p;
    auto normal = rec.normal;

    // rotate back the rotate direction is reverse
    p[0] = cos_theta*rec.p[0] + sin_theta *rec.p[2];
    p[2] = -sin_theta*rec.p[0] + cos_theta * rec.p[2];

    normal[0] = cos_theta*rec.normal[0] + sin_theta*rec.normal[2];
    normal[2] = -sin_theta*rec.normal[0] + cos_theta*rec.normal[2];

    rec.p = p;
    rec.set_face_normal(r, normal);

    return true;
}
bool RotateY::bounding_box(double time0, double time1, AABB& output_box) const{
    output_box = bbox;
    return hasbox;
}