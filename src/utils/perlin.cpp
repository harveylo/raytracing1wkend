#include "perlin.hpp"
#include "util.hpp"

Perlin::Perlin(){
    ranfloat = new double [point_count];

    for(int i = 0;i<point_count; i++){
        ranfloat[i] = random_double();
    }

    perm_x = perlin_generate_perm();
    perm_y = perlin_generate_perm();
    perm_z = perlin_generate_perm();
}