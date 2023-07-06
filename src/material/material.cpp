#include "material.hpp"
#include "vec3.hpp"
#include "util.hpp"


bool Lambertian::scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered, double& pdf) const{
        // Calculated the random point inside the unit sphere, Three ways to diffuse
        // Vec3 direction = Vec3::random_in_hemisphere(rec.normal); //uniform distribution, hemisphere
        // Vec3 direction = rec.normal + Vec3::random_in_unit_sphere(); // lazy hack, closer to normal
        Vec3 direction = rec.normal + Vec3::random_unit_vector(); // Lambertian Reflection, closer to scatter the ray
        if(direction.near_zero()) direction = rec.normal;

        scattered = Ray(rec.p,Vec3::unit_vector(direction),ray_in.time);
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        pdf = rec.normal.dot(scattered.direction())/PI;
        return true;
}

bool Metal::scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered, double& pdf) const {
    Vec3 direction = Vec3::unit_vector(ray_in.direction().reflect(rec.normal))+(fuzz*Vec3::random_in_unit_sphere());
    
    if(direction.near_zero()) direction = rec.normal;

    scattered = Ray(rec.p, direction, ray_in.time);
    attenuation = albedo;
    return true;
}

bool Dielectric::scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered,double& pdf) const{
    // The glass surface absorb nothing, always reflect or refract
    attenuation = Color(1.0,1.0,1.0);

    // If it's been hit from the outside, the retiao is from air to dielectric(1/ir), otherwise ir(ir/1.0)
    double refraction_ratio = rec.front_face?(1.0/ir) : ir;

    Vec3 unit_direction = Vec3::unit_vector(ray_in.direction());

    auto cos_theta = fmin((-unit_direction).dot(rec.normal),1.0);
    auto sin_theta = sqrt(1.0-cos_theta*cos_theta);

    if(refraction_ratio*sin_theta>1 
        || reflectance(cos_theta, refraction_ratio)>random_double()
    ){
        scattered = Ray(rec.p,unit_direction.reflect(rec.normal));
    }else {
        Vec3 frac_diretion = unit_direction.refract(rec.normal,refraction_ratio);
        scattered = Ray(rec.p,frac_diretion, ray_in.time);
    }

    return true;
}

double Dielectric::reflectance(double cosine, double ref_idx){
    // Schlick's approximation
    auto r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0) * pow((1-cosine),5);
}

