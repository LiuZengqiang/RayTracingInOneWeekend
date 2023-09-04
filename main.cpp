// TODO: 增加 diffuse 材料属性
// diffuse:扩散漫反射
#include <float.h>

#include <iostream>

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "interval.h"
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
  // 在场景中添加两个球
  world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
  // world.add(make_shared<sphere>(point3(1, 0, -1), 0.5));
  world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

  // 定义 世界场景

  camera cam;
  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 400;

  cam.render(world);

  return 0;
}