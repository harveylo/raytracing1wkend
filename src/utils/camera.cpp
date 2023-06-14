#include "camera.hpp"
#include "util.hpp"

Camera::Camera(){
    auto aspect_ratio = 16.0/9.0;
    auto view_height = 2.0;
    auto view_width = aspect_ratio*view_height;
    auto focal_length = 1.0;

    origin = Point3(0,0,0);
    horizontal = Vec3(view_width,0.0,0.0);
    vertical = Vec3(0.0,view_height,0.0);
    lower_left_corner = origin - 0.5*horizontal - 0.5 * vertical - Vec3(0,0,focal_length);
}

Camera::Camera(double vfov, double aspect_ratio){
    auto theta = degrees_to_radians(vfov);

    // The half of height of viewport on the z = -1 position
    auto h = tan(theta/2);
    auto view_height = 2.0*h;
    auto view_width = view_height*aspect_ratio;

    auto focal_length = 1.0;

    origin = Point3(0,0,0);
    horizontal = Vec3(view_width,0.0,0.0);
    vertical = Vec3(0.0,view_height,0.0);
    lower_left_corner = origin - 0.5*horizontal - 0.5 * vertical - Vec3(0,0,focal_length);


}


Camera::Camera(Point3 lookfrom, Point3 lookat, Vec3 vup, double vfov, double  aspect_ratio){
    auto theta = degrees_to_radians(vfov);

    // The half of height of viewport on the z = -1 position
    auto h = tan(theta/2);
    auto view_height = 2.0*h;
    auto view_width = view_height*aspect_ratio;

    auto w = Vec3::unit_vector(lookfrom-lookat);
    auto u = Vec3::unit_vector(vup.cross(w));
    auto v = w.cross(u);

    origin = lookfrom;
    horizontal = u*view_width;
    vertical = v*view_height;
    lower_left_corner = origin - w - horizontal/2 -vertical/2;
    
}