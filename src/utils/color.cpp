#include "color.hpp"
#include "sphere.hpp"
#include "util.hpp"

// desolation simple one-sample write_color function 
// void write_color(std::ostream &out, Color pixel_color){
//     // Write the translated [0,255] value of each color component.
//     out << static_cast<int>(256 * clamp(pixel_color.x(), 0, 0.999)) << ' '
//         << static_cast<int>(256 * clamp(pixel_color.y(), 0, 0.999)) << ' '
//         << static_cast<int>(256 * clamp(pixel_color.z(), 0, 0.999)) << '\n';
// }

void write_color(std::ostream &out, Color pixel_color, int samples_per_pixel){
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Calculate scale and the average
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;


    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
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