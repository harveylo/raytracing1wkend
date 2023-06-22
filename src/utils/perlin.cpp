#include "perlin.hpp"
#include "util.hpp"
#include <cmath>
#include <math.h>

static double trilinear_interp(double c[2][2][2], double u, double v,
                               double w) {
  auto accum = .0;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
        accum += (i * u + (1 - i) * (1 - u)) * (j * v + (1 - j) * (1 - v)) *
                 (k * w + (1 - k) * (1 - w)) * (c[i][j][k]);
      }
    }
  }
  return accum;
}

Perlin::Perlin() {
  ranfloat = new double[point_count];

  for (int i = 0; i < point_count; i++) {
    ranfloat[i] = random_double();
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
  double c[2][2][2];

  for (int x = 0; x < 2; x++) {
    for (int y = 0; y < 2; y++) {
      for (int z = 0; z < 2; z++) {
        c[x][y][z] = ranfloat[perm_x[(i + x) & 255] ^ perm_y[(j + y) & 255] ^
                              perm_z[(k + z) & 255]];
      }
    }
  }

  return trilinear_interp(c, u, v, w);
}