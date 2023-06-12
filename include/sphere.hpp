#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "vec3.hpp"
#include "ray.hpp"
#include "hittable.hpp"
#include <memory>
#include "material.hpp"

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