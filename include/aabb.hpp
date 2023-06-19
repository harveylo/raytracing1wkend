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

    bool hit(const Ray& r, double t_min, double t_max){
        for(int i = 0; i< 3; i++){
            auto t0 = fmin((minimum[i]-r.origin()[i])/r.direction()[i],
                           (maximum[i]-r.origin()[i])/r.direction()[i]);
            auto t1 = fmax((minimum[i]-r.origin()[i])/r.direction()[i],
                           (maximum[i]-r.origin()[i])/r.direction()[i]);
            t_min = fmax(t_min,t0);
            t_max = fmin(t_max,t1);
            if(t_max<=t_min) return false;
        }
        return true;
    }
    
};


#endif