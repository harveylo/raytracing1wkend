#include "color.hpp"


void write_color(std::ostream &out, Color pixel_color){
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

Color ray_color(const Ray& r) {
    // Get the unit direction vector of the ray
    Vec3 unit_direction = Vec3::unit_vector(r.direction());
    // Select t according to the y coordinate of the unit direction vector
    auto t = 0.5*(unit_direction.y() + 1.0);
    // Return the linear blend of white and blue
    return (1.0-t)*Color(1.0,1.0,1.0) + t*Color(0.5,0.7,1.0);
}