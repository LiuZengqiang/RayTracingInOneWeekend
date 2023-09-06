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

int main() {
  /* 生成场景 */
  hittable_list world;
  auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();
      point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
      // 根据随机数 choose_mat 的取值大小随机生成 diffuse, metal, glass 材质的球
      if ((center - point3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<material> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse, 漫反射材质, 反射光线在交点法线方向存在散射(漫反射)和衰减
          auto albedo = color::random() * color::random();
          sphere_material = make_shared<lambertian>(albedo);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal, 金属材质, 反射光线在理想反射光线方向存在散射(漫反射)和衰减
          auto albedo = color::random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          sphere_material = make_shared<metal>(albedo, fuzz);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else {
          // glass, 玻璃材质, 反射光线存在折射,反射和衰减
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

  /* 设置相机和输出图像的属性 */
  camera cam;
  cam.aspect_ratio = 16.0 / 9.0;  // 图像的长宽比
  cam.image_width = 400;          // 图像的宽(像素数)
  cam.samples_per_pixel = 5;      // 每个像素的采样光线数
  cam.max_depth = 10;             // 光线的最大深度

  cam.vfov = 20;                    // 视场角
  cam.lookfrom = point3(13, 2, 3);  // 相机位置
  cam.lookat = point3(0, 0, 0);     // 相机观察的点
  cam.vup = vec3(0, 1, 0);          // 相机上方向向量

  cam.defocus_angle = 0.6;  // 模拟实际相机的散射角度(以实现景深效果)
  cam.focus_dist = 10.0;  // 模拟实际相机的理想焦距(以实现景深效果)

  cam.render(world);

  return 0;
}