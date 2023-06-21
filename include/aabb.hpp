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
    
    AABB() {}
    AABB(const Point3& minimum, const Point3& maximum): minimum(minimum),maximum(maximum){}

    inline bool hit(const Ray& r, double t_min, double t_max) const{
        for(int i = 0; i< 3; i++){
            // auto t0 = fmin((minimum[i]-r.origin()[i])/r.direction()[i],
            //                (maximum[i]-r.origin()[i])/r.direction()[i]);
            // auto t1 = fmax((minimum[i]-r.origin()[i])/r.direction()[i],
            //                (maximum[i]-r.origin()[i])/r.direction()[i]);
            // t_min = fmax(t_min,t0);
            // t_max = fmin(t_max,t1);
            // if(t_max<=t_min) return false;
            //  optimized code as below
            auto invD = 1.0f / r.direction()[i];
            auto t0 = (minimum[i] - r.origin()[i]) * invD;
            auto t1 = (maximum[i] - r.origin()[i]) * invD;
            if(invD < 0.0f) std::swap(t0,t1);
            t_min = t0 > t_min? t0: t_min;
            t_max = t1 < t_max? t1: t_max;
            if(t_max <= t_min) return false;
        }
        return true;
    }

    static AABB surrounding_box(const AABB& a, const AABB& b){
        Point3 low(fmin(a.min()[0],b.min()[0]),
                  fmin(a.min()[1],b.min()[1]),
                  fmin(a.min()[2],b.min()[2]));
        Point3 high(fmax(a.max()[0],b.max()[0]),
                    fmax(a.max()[1],b.max()[1]),
                    fmax(a.max()[2],b.max()[2]));
        return AABB(low,high);
    }
    
};


#endif