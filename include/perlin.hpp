#ifndef __PERLIN_H__
#define __PERLIN_H__

#include "util.hpp"
#include "vec3.hpp"

class Perlin{
private:
    static constexpr int point_count = 256;
    // ! Why using heap memory(pointer), not stack?
    Vec3* ranvec;
    int* perm_x;
    int* perm_y;
    int* perm_z;

    static int* perlin_generate_perm(){
        // generate a random swapped array;
        auto p = new int[point_count];
        for(int i = 0;i<point_count;i++) p[i]=i;

        permute(p,point_count);
        
        return p;
    }

    static void permute(int* p, int n){
        // randomly sawp n times
        for(int i = n-1; i>0;i--){
            int target = random_int_inc(0, i);
            p[i]^=p[target];
            p[target]^=p[i];
            p[i]^=p[target];
        }
    }

public:
    // Perlin constructor will initialize the four random  arrays
    Perlin();
    ~Perlin(){
        delete[] ranvec;
        delete [] perm_x;
        delete [] perm_y;
        delete [] perm_z;
    }
    double noise(const Point3& p) const;

    double turb(const Point3& p, int depth=7) const;
};

#endif