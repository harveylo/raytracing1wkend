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
    Vec3 u,v,w;
public:
    // Default(No Arguments) constructor will construct a camera at world origin, looking at -z
    Camera();
    // vfov is vertical fov in degrees
    Camera(double vfov, double aspect_ratio);

    Camera(Point3 lookfrom, Point3 lookat, Vec3 vup, double vfov, double  aspect_ratio);
    Ray get_ray(double s, double t) const{
        return Ray(origin,lower_left_corner+s*horizontal+t*vertical-origin);
    }
};

#endif