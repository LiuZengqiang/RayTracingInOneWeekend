#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
 public:
  ray() {}

  ray(const point3& origin, const vec3& direction)
      : orig(origin), dir(direction) {}
  // 取 ray 的原点
  point3 origin() const { return orig; }
  // 取 ray 的方向
  vec3 direction() const { return dir; }
  // 取 ray 的终点
  point3 at(double t) const { return orig + t * dir; }

 private:
  point3 orig;
  vec3 dir;
};

#endif