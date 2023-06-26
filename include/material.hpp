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

    virtual Color emitted(double u, double v, const Point3& p) const{
        // emit black light by default
        return Color(0,0,0);
    }
    virtual ~Material() = default;
};

class Lambertian : public Material{
public:
    std::shared_ptr<Texture> albedo;


    // The fraction of sunlight that is fiffusely reflected by the body 
    Lambertian(const Color& a): albedo(std::make_shared<SolidColor>(a)){};
    Lambertian(std::shared_ptr<Texture> a): albedo(a){}
    virtual bool scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override;
    virtual ~Lambertian() = default;
    
};


class Metal: public Material{
public:
    Color albedo;
    double fuzz;
    Metal(const Color& a): albedo(a),fuzz(0){};
    Metal(const Color&a, double f): albedo(a),fuzz(f){};
    virtual bool scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override;
    virtual ~Metal() = default;
};

class Dielectric: public Material{
public:
    double ir;

    Dielectric(double ir):ir(ir){}
    virtual bool scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override;
    virtual ~Dielectric() = default;
private:
    static double reflectance(double cosine, double ref_idx);
};

class DiffuseLight : public Material {
public:
    std::shared_ptr<Texture> emit;

    DiffuseLight(std::shared_ptr<Texture> emit):emit(emit) {}
    DiffuseLight(Color c) : emit(std::make_shared<SolidColor>(c)) {}

    virtual bool scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override{
        // a light emission object will not scatter a ray
        return false;
    }

    virtual Color emitted(double u, double v, const Point3& p) const override {
        return emit -> value(u, v, p);
    }

    virtual ~DiffuseLight() = default;
};

class Isotropic : public Material {
public:
    std::shared_ptr<Texture> albedo;

    Isotropic(Color c) : albedo(std::make_shared<SolidColor>(c)) {}
    Isotropic(std::shared_ptr<Texture> a): albedo(a) {}

    virtual bool scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override{
        // Scatter inside the volume is completely random in direction
        scattered = Ray(rec.p,Vec3::random_in_unit_sphere(),ray_in.time);

        attenuation = albedo -> value(rec.u, rec.v, rec.p);

        return true;
    }
};
#endif