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
        // Calculated the random point inside the unit sphere, Three ways to diffuse
        // Point3 target = rec.p+Vec3::random_in_hemisphere(rec.normal); //uniform distribution, hemisphere
        // Point3 target = rec.p+ rec.normal + Vec3::random_in_unit_sphere(); // lazy hack, closer to normal
        Point3 target = rec.p+ rec.normal + Vec3::random_unit_vector(); // Lambertian Reflection, closer to scatter the ray
        

        // Get the color of the child ray, but only has half weight.
        // with every hit, the contribution will be decreased to a half
        // At last, the color would be the color of the world or zero
        return 0.5 * ray_color(Ray(rec.p,target - rec.p),world,depth-1);
    }
    // Return the linear blend of white and blue
    Vec3 unit_direction = Vec3::unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y()+1.0);
    return (1.0-t)*Color(1.0,1.0,1.0) + t*Color(0.5,0.7,1.0);
}