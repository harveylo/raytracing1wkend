#include "perlin.hpp"
#include "util.hpp"
#include <chrono>
#include <cmath>
#include <math.h>

static double perlin_interp(Vec3 c[2][2][2], double u, double v,
                               double w) {

  // hermite cube to smoothe the interpolation
  // can also use cosine like (1 - cos(t*pi)) / 2
  // Ken Perlin suggest using y = 6*t^5 - 15*t^4 + 10*t^3
  auto uu = u*u*(3-2*u);
  auto vv = v*v*(3-2*v);
  auto ww = w*w*(3-2*w);
  auto accum = .0;

  // interpolation
  for(int i = 0;i<2;i++){
    for(int j = 0;j<2;j++){
      for(int k = 0;k<2;k++){
        Vec3 weight_v(u-i,v-j,w-k);
        accum += (i*uu + (1-i)*(1-uu))*
                 (j*vv + (1-j)*(1-vv))*
                 (k*ww + (1-k)*(1-ww))*
                 (c[i][j][k].dot(weight_v));
      }
    }
  }
  return accum;
}

Perlin::Perlin() {
  ranvec = new Vec3[point_count];

  for (int i = 0; i < point_count; i++) {
    ranvec[i] = Vec3::unit_vector(Vec3::random(-1,1));
  }

  perm_x = perlin_generate_perm();
  perm_y = perlin_generate_perm();
  perm_z = perlin_generate_perm();
}

double Perlin::noise(const Point3 &p) const {
  auto u = p.x() - floor(p.x());
  auto v = p.y() - floor(p.y());
  auto w = p.z() - floor(p.z());

  // Hermite cube
  u = u*u*(3-2*u);
  v = v*v*(3-2*v);
  w = w*w*(3-2*w);

  auto i = static_cast<int>(floor(p.x()));
  auto j = static_cast<int>(floor(p.y()));
  auto k = static_cast<int>(floor(p.z()));
  Vec3 c[2][2][2];

  for (int x = 0; x < 2; x++) {
    for (int y = 0; y < 2; y++) {
      for (int z = 0; z < 2; z++) {
        c[x][y][z] = ranvec[perm_x[(i + x) & 255] ^ perm_y[(j + y) & 255] ^
                              perm_z[(k + z) & 255]];
      }
    }
  }

  return perlin_interp(c, u, v, w);
}


double Perlin::turb(const Point3& p, int depth) const{
    auto accum = 0.0;
    auto temp_p = p;
    auto weight = 1.0;

    for(int i =- 0;i<depth;i++){
      accum += weight*noise(temp_p);
      weight *= 0.5;
      temp_p *= 2;
    }

    return fabs(accum);
}