#include "color.hpp"
#include "bvh.hpp"
#include "hittable.hpp"
#include "util.hpp"
#include "material.hpp"
#include "camera.hpp"
#include "vec3.hpp"

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

Color ray_color(const Ray& r, const Color& background, const Hittable& world, int depth) {
    // If already reached the maximum depth, return no contribution color.
    if(depth <= 0) return Color(0,0,0);
    HitRecord rec;

    // if hits nothing, return background color
    if(!world.hit(r, 0.001, INFIN_D, rec))
        return background;
    //calculate child ray according to material
    Color attenuation;
    Ray scattered;
    Color emitted = rec.mat_ptr -> emitted(rec.u, rec.v, rec.p);

    // if no scattered ray, return only the emitted ray color
    if(!(rec.mat_ptr)->scatter(r,rec,attenuation,scattered)) 
        return emitted;

    return emitted + attenuation * ray_color(scattered, background, world, depth-1);
}

void scanline_render(const int line,
                     const Camera& cam,
                     const BVHNode& world,
                     const int image_width,
                     const int image_height,
                     const int samples_per_pixel,
                     const int max_depth,
                     const Color& background,
                     std::vector<std::vector<std::vector<int>>>& image,
                     std::counting_semaphore<MAX_BATCH>& sem){
    for(int i = 0;i<image_width;i++){
        Color pixel_color(0,0,0);
        for(int j = 0;j<samples_per_pixel;j++){
            auto u = (i+random_double())/(image_width-1);
            auto v = (line+random_double())/(image_height-1);
            Ray r = cam.get_ray(u,v);
            pixel_color += ray_color(r,background,world,max_depth);
        }

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
        image[line][i][0]=static_cast<int>(256 * clamp(r, 0.0, 0.999));
        image[line][i][1]=static_cast<int>(256 * clamp(g, 0.0, 0.999));
        image[line][i][2]=static_cast<int>(256 * clamp(b, 0.0, 0.999));
    }
    sem.release();

}