#include "color.hpp"
#include "sphere.hpp"
#include "util.hpp"


void write_color(std::ostream &out, Color pixel_color){
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

Color ray_color(const Ray& r, const HittableList& world) {
    HitRecord rec;
    if(world.hit(r,0,INFIN_D,rec)){
        return 0.5*(rec.normal+Color(1,1,1));
    }
    // Return the linear blend of white and blue
    Vec3 unit_direction = Vec3::unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y()+1.0);
    return (1.0-t)*Color(1.0,1.0,1.0) + t*Color(0.5,0.7,1.0);
}