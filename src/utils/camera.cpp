#include "camera.hpp"

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