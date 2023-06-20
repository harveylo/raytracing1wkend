#include "texture.hpp"
#include <cmath>


Color CheckerTexture::value(double u, double v, const Point3& p) const{
    auto sines = sin(10*p.x()*sin(10*p.y())*sin(10*p.z()));
    if(sines<0) return odd->value(u, v, p);
    else return even->value(u, v, p);
}

