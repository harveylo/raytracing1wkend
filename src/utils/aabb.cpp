#include "aabb.hpp"
#include "ray.hpp"





inline bool AABB::hit(const Ray& r, double t_min, double t_max) const{
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