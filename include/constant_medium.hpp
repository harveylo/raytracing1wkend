#ifndef __CONSTANT_MEDIUM_H__
#define __CONSTANT_MEDIUM_H__

#include "material.hpp"
#include "texture.hpp"
#include "hittable.hpp"
#include "vec3.hpp"


class ConstantMedium : public Hittable{
public:
    // boundary of the volume. For a volume, it's boundary and actual hit function is not the same.
    std::shared_ptr<Hittable> boundary;
    std::shared_ptr<Material> phase_function;
    // The inversion of the density, inversed for convenience
    // The probability function will use log
    double neg_inv_density;

    ConstantMedium(std::shared_ptr<Hittable>b, double d, std::shared_ptr<Texture> a): boundary(b), phase_function(std::make_shared<Isotropic>(a)), neg_inv_density(-1/d) {}
    ConstantMedium(std::shared_ptr<Hittable>b, double d, Color c): boundary(b), phase_function(std::make_shared<Isotropic>(c)), neg_inv_density(-1/d) {}

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override{
        return boundary -> bounding_box(time0, time1, output_box);
    }

    virtual ~ConstantMedium() = default;

};

#endif