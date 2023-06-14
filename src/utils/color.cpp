#include "color.hpp"
#include "util.hpp"
#include "material.hpp"

// desolation simple one-sample write_color function 
// void write_color(std::ostream &out, Color pixel_color){
//     // Write the translated [0,255] value of each color component.
//     out << static_cast<int>(256 * clamp(pixel_color.x(), 0, 0.999)) << ' '
//         << static_cast<int>(256 * clamp(pixel_color.y(), 0, 0.999)) << ' '
//         << static_cast<int>(256 * clamp(pixel_color.z(), 0, 0.999)) << '\n';
// }

using std::sqrt;

void write_color(std::ostream &out, Color pixel_color, int samples_per_pixel){
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Calculate scale and the average
    auto scale = 1.0 / samples_per_pixel;

    // Gamma correction
    // Due to r, g and b are all less than 1, sqrt will increase their value
    r = sqrt(scale*r);
    g = sqrt(scale*g);
    b = sqrt(scale*b);


    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

Color ray_color(const Ray& r, const HittableList& world, int depth) {
    // If already reached the maximum depth, return no contribution color.
    if(depth <= 0) return Color(0,0,0);
    HitRecord rec;
    if(world.hit(r,0.001,INFIN_D,rec)){
        //calculate child ray according to material
        Color attenuation;
        Ray scattered;
        if((rec.mat_ptr)->scatter(r,rec,attenuation,scattered)) 
            return attenuation*(ray_color(scattered,world,depth-1));
        return Color(0,0,0); 
    }
    // Return the linear blend of white and blue
    Vec3 unit_direction = Vec3::unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y()+1.0);
    return (1.0-t)*Color(1.0,1.0,1.0) + t*Color(0.5,0.7,1.0);
}