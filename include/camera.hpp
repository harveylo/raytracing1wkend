#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "util.hpp"
#include "vec3.hpp"
#include "ray.hpp"

class Camera{
private:
    Point3 origin;
    Point3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
public:
    Camera();
    Ray get_ray(double u, double v) const{
        return Ray(origin,lower_left_corner+u*horizontal+v*vertical-origin);
    }
};

#endif