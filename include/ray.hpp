#ifndef __RAY_H__
#define __RAY_H__

#include "vec3.hpp"

class Ray{
public:
    Point3 oriPoint;
    Vec3 dirVec;
    Ray(){}
    Ray(const Point3& origin, const Vec3& direction): oriPoint(origin), dirVec(direction){}
    Point3 origin() const {return oriPoint;}
    Vec3 direction() const {return dirVec;}
    Point3 at(double t) const {return oriPoint + t*dirVec;}
};

#endif