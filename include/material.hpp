#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "util.hpp"
#include "hittable.hpp"
#include "ray.hpp"


class Material{
public:
    //pure virtual funciton
    virtual bool scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scatterd) const = 0;
};

#endif