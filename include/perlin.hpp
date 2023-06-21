#ifndef __PERLIN_H__
#define __PERLIN_H__

#include "util.hpp"
#include "vec3.hpp"

class Perlin{
private:
    static constexpr int point_count = 256;
    double* ranfloat;
    int* perm_x;
    int* perm_y;
    int* perm_z;

    static int* perlin_generate_perm(){
        auto p = new int[point_count];
        for(int i = 0;i<point_count;i++) p[i]=i;

        permute(p,point_count);
        
        return p;
    }

    static void permute(int* p, int n){
        for(int i = n-1; i>0;i--){
            int target = random_int_inc(0, i);
            p[i]^=p[target];
            p[target]^=p[i];
            p[i]^=p[target];
        }
    }

public:
    Perlin();
    ~Perlin(){
        delete[] ranfloat;
        delete [] perm_x;
        delete [] perm_y;
        delete [] perm_z;
    }
    double noise(const Point3& p) const{
        auto i = static_cast<int>(4*p.x()) & 255;
        auto j = static_cast<int>(4*p.y()) & 255;
        auto k = static_cast<int>(4*p.z()) & 255;

        return ranfloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]];

    }
};

#endif