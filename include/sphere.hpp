#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "vec3.hpp"
#include "ray.hpp"
#include "hittable.hpp"

class Sphere: public Hittable{
public:
    Point3 center;
    double radius;
    Sphere(){}
    Sphere(Point3 cen, double r):center(cen),radius(r){}
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
};


#endif