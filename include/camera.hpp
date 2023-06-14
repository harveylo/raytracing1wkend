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
    double lens_radius;
public:
    // Default(No Arguments) constructor will construct a camera at world origin, looking at -z
    Camera();
    // vfov is vertical fov in degrees
    Camera(double vfov, double aspect_ratio);

    Camera(Point3 lookfrom, Point3 lookat, Vec3 vup, double vfov, double  aspect_ratio);

    // lookat is just for dscribing the direction with lookfrom, focus_dist specify the focus distance
    // Though we can use lookat and lookfrom for both the dicrection and the focus distance, but the flexibility will be impaired
    Camera(Point3 lookfrom, Point3 lookat, Vec3 vup, double vfov, double  aspect_ratio, double aperture, double focus_dist);
    Ray get_ray(double s, double t) const{
        // Get a vector in disk
        Vec3 rd = lens_radius * Vec3::random_in_unit_disk();
        // compose each vector projection on u and v to form the fianl ray starting point
        Vec3 offset = u*rd.x()+v*rd.y();
        return Ray(origin+offset,lower_left_corner+s*horizontal+t*vertical-origin-offset);
    }
};

#endif