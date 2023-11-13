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

int main()
{
    /* 生成场景 */
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    /* 设置相机和输出图像的属性 */
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0; // 图像的长宽比
    cam.image_width = 400;         // 图像的宽(像素数)
    cam.samples_per_pixel = 50;    // 每个像素的采样光线数
    cam.max_depth = 10;            // 光线的最大深度

    cam.vfov = 90;                  // 视场角
    cam.lookfrom = point3(0, 0, 0); // 相机位置
    cam.lookat = point3(0, 0, -1);  // 相机观察的点
    cam.vup = vec3(0, 1, 0);        // 相机上方向向量

    cam.defocus_angle = 0.6; // 模拟实际相机的散射角度(以实现景深效果)
    cam.focus_dist = 1.0;    // 模拟实际相机的理想焦距(以实现景深效果)

    cam.render(world);

    return 0;
}