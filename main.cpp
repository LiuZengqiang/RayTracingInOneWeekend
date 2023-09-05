// TODO:增加材料类materials
// diffuse:扩散漫反射
#include <float.h>

#include <iostream>

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "interval.h"
#include "material.h"
#include "ray.h"
#include "rtweekend.h"
#include "sphere.h"
#include "vec3.h"

// 在场景中加一个球
// 球 是那些 到点C距离为r的点的集合
// 即球S上的点P满足 (P-C)*(P-C) = r^2
// 如果光线P(t)=A+t*B 与球S,相交应该满足
// (P(t)-C)*(P(t)-C)=r^2 公式(1)
// (A+t*B-C)*(A+t*B-C)=r^2 公式(2)
// t*t*B*B+2t*B*(A-C)+(A-C)*(A-C)-r^2=0 公式(3)
// 未知数为t, 若公式(3)有解，需要改一元二次方程有实数解，那么
// 该一元二次方程的判别公式大于等于0，
// 可以使用求根公式得到是否有解t，解t的个数

// 简化 求教运算
// b -> 2*h
// r*r -> r.square

int main() {
  hittable_list world;
  // 定义 世界场景
  auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
  auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
  auto material_left = make_shared<dielectric>(1.5);
  auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

  world.add(
      make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
  world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
  world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
  world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.4, material_left));
  world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));
  
  camera cam;
  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth = 50;

  cam.vfov = 20;
  cam.lookfrom = point3(-2, 2, 1);
  cam.lookat = point3(0, 0, -1);
  cam.vup = vec3(0, 1, 0);

  cam.render(world);

  return 0;
}