#ifndef __UTIL_H__
#define __UTIL_H__

#include <cmath>
#include <limits>
#include <memory>

// Constants
const double INFIN_D = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932384626;

// Utility Functions
inline double degrees_to_radians(double degrees){
    return degrees * PI / 180.0;
}

#endif