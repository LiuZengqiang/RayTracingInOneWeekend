#ifndef COLOR_H
#define COLOR_H

#include <iostream>

#include "rtweekend.h"
#include "vec3.h"

using color = vec3;

// samples_per_pixel 是一个像素的采样光线数
// 最终的像素颜色 等于 多根采样光线的平均
void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
  // Write the translated [0,255] value of each color component.

  auto r = pixel_color.x();
  auto g = pixel_color.y();
  auto b = pixel_color.z();

  auto scale = 1.0 / samples_per_pixel;
  r *= scale;
  g *= scale;
  b *= scale;
  static const interval intensity(0.0, 0.999);

  out << static_cast<int>(255.999 * intensity.clamp(r)) << ' '
      << static_cast<int>(255.999 * intensity.clamp(g)) << ' '
      << static_cast<int>(255.999 * intensity.clamp(b)) << '\n';
}

#endif
