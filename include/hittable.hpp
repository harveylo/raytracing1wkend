#ifndef __HITTABLE_H__
#define __HITTABLE_H__

#include "ray.hpp"
#include <memory>
#include "vec3.hpp"

class Material;

struct HitRecord {
    Point3 p;
    Vec3 normal;
    double t;
    bool front_face;
    std::shared_ptr<Material> mat_ptr;
    HitRecord():p(),normal(),t(),front_face(){}
    HitRecord(const Point3& p, const Vec3& normal, double t):p(p),normal(normal),t(t){}
    void set_record(const Point3& p, double t){
        this->p = p;
        this->t = t;
    }
    void set_face_normal(const Ray& r, const Vec3& outward_normal){
        front_face = outward_normal.dot(r.direction())<0;
        normal = front_face?outward_normal:-outward_normal;
    }
    void set_material(std::shared_ptr<Material> m){
        mat_ptr = m;
    }
};

class Hittable {
public:
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
};

class Sphere: public Hittable{
public:
    Point3 center;
    double radius;
    std::shared_ptr<Material> mat_ptr;

    Sphere(){}
    Sphere(Point3 cen, double r):center(cen),radius(r){}
    Sphere(Point3 cen, double r, std::shared_ptr<Material> m):center(cen),radius(r),mat_ptr(m){}
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
};

#endif