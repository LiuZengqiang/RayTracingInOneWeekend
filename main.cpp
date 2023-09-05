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
  auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();
      point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

      if ((center - point3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<material> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = color::random() * color::random();
          sphere_material = make_shared<lambertian>(albedo);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = color::random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          sphere_material = make_shared<metal>(albedo, fuzz);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = make_shared<dielectric>(1.5);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<dielectric>(1.5);
  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

  camera cam;
  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 200;
  cam.samples_per_pixel = 5;
  cam.max_depth = 10;

  cam.vfov = 20;
  cam.lookfrom = point3(13, 2, 3);
  cam.lookat = point3(0, 0, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0.6;
  cam.focus_dist = 10.0;

  cam.render(world);

  return 0;
}