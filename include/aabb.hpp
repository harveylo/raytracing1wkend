#ifndef __AABB_H__
#define __AABB_H__

#include "util.hpp"
#include "vec3.hpp"
#include "ray.hpp"

class AABB {
public:
    // A point stores the 3 dimensions' boundery value
    Point3 minimum;
    Point3 maximum;

    Point3 min() const {return minimum;}
    Point3 max() const {return maximum;}

    inline bool hit(const Ray& r, double t_min, double t_max) const ;

    static AABB surrounding_box(const AABB& a, const AABB& b){
        Point3 low(fmin(a.min()[0],b.min()[0]),
                  fmin(a.min()[1],b.min()[1]),
                  fmin(a.min()[2],b.min()[2]));
        Point3 high(fmax(a.max()[0],b.max()[1]),
                    fmax(a.max()[1],b.max()[1]),
                    fmax(a.max()[2],b.max()[2]));
        return AABB(low,high);
    }
    
};


#endif