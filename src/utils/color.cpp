#include "color.hpp"
#include "sphere.hpp"
#include "util.hpp"
#include "hittable.hpp"


void write_color(std::ostream &out, Color pixel_color){
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

Color ray_color(const Ray& r) {
    //calculate the root if r can hit the sphere
    auto t = hit_sphere(Point3(0,0,-1),0.5,r);
    if(t>0.0){
        // if r hits the sphere, get the normal vector of the sphere at the hit point calculated by t
        Vec3 N = Vec3::unit_vector(r.at(t) - Vec3(0,0,-1));
        // return the color of the sphere at the hit point
        // every dimension of the normal is -1 to 1, map it to 0 to 1
        return 0.5*Color(N.x()+1,N.y()+1,N.z()+1);
    }
    // if not hit the sphere, return the background color
    Vec3 unit_direction = Vec3::unit_vector(r.direction());
    // Select t according to the y coordinate of the unit direction vector
    t = 0.5*(unit_direction.y() + 1.0);
    // Return the linear blend of white and blue
    return (1.0-t)*Color(1.0,1.0,1.0) + t*Color(0.5,0.7,1.0);
}