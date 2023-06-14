#include "camera.hpp"
#include "util.hpp"

Camera::Camera(){
    auto aspect_ratio = 16.0/9.0;
    auto view_height = 2.0;
    auto view_width = aspect_ratio*view_height;
    auto focal_length = 1.0;

    w = Vec3(0,0,-1);
    u = Vec3(1,0,0);
    v = Vec3(0,1,0);
    
    origin = Point3(0,0,0);
    horizontal = Vec3(view_width,0.0,0.0);
    vertical = Vec3(0.0,view_height,0.0);
    lower_left_corner = origin - 0.5*horizontal - 0.5 * vertical - Vec3(0,0,focal_length);
    lens_radius = 0.0;
}

Camera::Camera(double vfov, double aspect_ratio){
    auto theta = degrees_to_radians(vfov);

    // The half of height of viewport on the z = -1 position
    auto h = tan(theta/2);
    auto view_height = 2.0*h;
    auto view_width = view_height*aspect_ratio;

    auto focal_length = 1.0;

    w = Vec3(0,0,-1);
    u = Vec3(1,0,0);
    v = Vec3(0,1,0);

    origin = Point3(0,0,0);
    horizontal = Vec3(view_width,0.0,0.0);
    vertical = Vec3(0.0,view_height,0.0);
    lower_left_corner = origin - 0.5*horizontal - 0.5 * vertical - Vec3(0,0,focal_length);

    lens_radius = 0.0;
}


Camera::Camera(Point3 lookfrom, Point3 lookat, Vec3 vup, double vfov, double  aspect_ratio){
    auto theta = degrees_to_radians(vfov);

    // The half of height of viewport on the z = -1 position
    auto h = tan(theta/2);
    auto view_height = 2.0*h;
    auto view_width = view_height*aspect_ratio;

    w = Vec3::unit_vector(lookfrom-lookat);
    u = Vec3::unit_vector(vup.cross(w));
    v = w.cross(u);

    origin = lookfrom;
    horizontal = u*view_width;
    vertical = v*view_height;
    lower_left_corner = origin - w - horizontal/2 -vertical/2;
    
    lens_radius = 0.0;
}


Camera::Camera(Point3 lookfrom, Point3 lookat, Vec3 vup, double vfov, double  aspect_ratio, double aperture, double focus_dist){
    auto theta = degrees_to_radians(vfov);

    // The half of height of viewport on the z = -1 position
    auto h = tan(theta/2);
    auto view_height = 2.0*h;
    auto view_width = view_height*aspect_ratio;

    w = Vec3::unit_vector(lookfrom-lookat);
    u = Vec3::unit_vector(vup.cross(w));
    v = w.cross(u);

    origin = lookfrom;
    // The viewport now is not at z = -1, it's at the focused plane
    // Thus scale up the u and v according to focus_dist
    horizontal = focus_dist*view_width*u;
    vertical = focus_dist*view_height*v;
    lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

    lens_radius = aperture/2;
}