#include "cmath"
#include "sphere.hpp"

double hit_sphere(const Point3& center, double radius, const Ray& r){
    Vec3 a_c = r.origin() - center;
    auto a = r.direction().dot(r.direction());
    auto b = 2.0 * r.direction().dot(a_c);
    auto c = a_c.dot(a_c) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    if(discriminant<0) return -1.0;
    else return (-b - sqrt(discriminant))/(2.0*a);
}