#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "util.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include "hittable.hpp"
#include "texture.hpp"
#include <memory>

class Material{
public:
    //pure virtual funciton
    virtual bool scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material{
public:
    std::shared_ptr<Texture> albedo;


    // The fraction of sunlight that is fiffusely reflected by the body 
    Lambertian(const Color& a): albedo(std::make_shared<SolidColor>(a)){};
    Lambertian(std::shared_ptr<Texture> a): albedo(a){}
    virtual bool scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override;
    
};


class Metal: public Material{
public:
    Color albedo;
    double fuzz;
    Metal(const Color& a): albedo(a),fuzz(0){};
    Metal(const Color&a, double f): albedo(a),fuzz(f){};
    virtual bool scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override;
};

class Dielectric: public Material{
public:
    double ir;

    Dielectric(double ir):ir(ir){}
    virtual bool scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override;
private:
    static double reflectance(double cosine, double ref_idx);
};


#endif