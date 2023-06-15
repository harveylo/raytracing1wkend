#ifndef __UTIL_H__
#define __UTIL_H__

#include <cmath>
#include <limits>
#include <memory>

// Constants
const double INFIN_D = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932384626;
constexpr int BATCH=8;

// Utility Functions
inline double degrees_to_radians(double degrees){
    return degrees * PI / 180.0;
}
inline double random_double(){
    // Returns a random double in [0,1)
    return rand()/(RAND_MAX + 1.0);
}

inline double random_double(double min, double max){
    // Returns a random double in [min,max)
    return min + (max-min)*random_double();
}

// This function clamp the x to [min,max]
inline double clamp(double x, double min, double max){
    return x<min?min:(x>max?max:x);
}

#endif