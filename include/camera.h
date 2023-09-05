#ifndef CAMERA_H
#define CAMERA_H
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "rtweekend.h"

// 相机类
class camera {
 public:
  double aspect_ratio = 16.0 / 9.0;  // 图片的横纵比例
  int image_width = 400;             // 图片的像素宽度
  int samples_per_pixel = 100;       // 每个像素的采样光线数
  int max_depth = 10;                // 光线在最大深度(反射次数)
  /* Public Camera Parameters Here */

  void render(const hittable_list& world) {
    // 先初始化相机的各个参数
    initialize();
    // Render
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    // 渲染图像
    for (int j = 0; j < image_height; ++j) {
      std::clog << "\rScanlines remaining: " << (image_height - j) << ' '
                << std::flush;
      for (int i = 0; i < image_width; ++i) {
        color pixel_color;
        for (int sample = 0; sample < samples_per_pixel; sample++) {
          // 采样得到一条 视点->图像(i,j)像素的光线，在像素(i,j)中随机采样
          auto r = get_ray(i, j);
          // 在这里实现一个 计算光线 r 颜色的光线跟踪程序
          pixel_color += ray_color(r, max_depth, world);
        }
        write_color(std::cout, pixel_color, samples_per_pixel);
      }
    }

    std::clog << "\rDone.                 \n";
  }

 private:
  int image_height;    // 图片的像素高度
  point3 center;       // 相机中心点
  point3 pixel00_loc;  // 左上角像素的坐标
  vec3 pixel_delta_u;  // u方向 单位像素的 坐标delta
  vec3 pixel_delta_v;  // v方向 单位像素的 坐标delta

  /* Private Camera Variables Here */
  void initialize() {
    // Calculate the image height, and ensure that it's at least 1.
    // image 是视窗(viewport) 映射到屏幕(位图)上的二维矩阵
    image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera
    // 相机参数
    auto focal_length = 1.0;  // 焦距 表示 视点 到 视窗 的距离
    // Viewport widths less than one are ok since they are real valued.
    // viewport 视窗, 是一个虚拟的窗口，使用世界坐标系下的坐标尺寸表示
    // 这里的 viewport_height 和 viewport_width 必须用实际的 image
    // 像素值计算，不能用 aspect_ratio 计算，因为 image_width/image_height
    // 不一定真的等于 aspect_ratio
    auto viewport_height = 2.0;  // 视窗
    auto viewport_width =
        viewport_height * (static_cast<double>(image_width) / image_height);
    center = point3(0, 0, 0);  // 视点(相机中心)

    // Calculate the vectors across the horizontal and down the vertical
    // viewport edges. 视窗u向量
    auto viewport_u = vec3(viewport_width, 0, 0);
    // 视窗v向量
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    // 计算每个像素代表的坐标delta
    // pixel_delta_u 等于 u 方向一个像素表示的坐标 delta 值
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    // 计算 视窗左上角的世界坐标
    // viewport_upper_left = 视点 往z负方向移动到达 视窗中心
    // 再往左移动视窗宽度的一半 再往上移动视窗高度的一半
    auto viewport_upper_left =
        center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;

    // 视窗图像像素坐标为(0,0)的世界坐标值
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
  }

  // 计算 光线 r 在场景 world 中的颜色
  color ray_color(const ray& r, int depth, const hittable_list& world) {
    if (depth <= 0) {
      return color(0, 0, 0);
    }
    hit_record rec;
    // 如果集中物体，就反射物体的颜色*0.5
    // 忽略[0,0.001) 范围内的交点，以避免浮点运算的误差
    if (world.hit(r, interval(0.001, infinity), rec)) {
      // 计算反射结果
      // 反射光线
      ray scattered;
      // 光线的散射
      color attenuation;
      // 入射光, 交点, 衰减, 散射
      if (rec.mat->scatter(r, rec, attenuation, scattered)) {
        return attenuation * ray_color(scattered, depth - 1, world);
      } else {
        // 理论上不能到达这里， rec.mat->scatter() 函数总是返回true
        return color(0, 0, 0);
      }
    }
    // 如果没有击中物体，就假设击中天空，天空的颜色是一个无意义的随机值
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
  }

  // 采样 视点->像素 (i,j) 的一条光线
  ray get_ray(int i, int j) const {
    auto pixel_center = pixel00_loc + i * pixel_delta_u + j * pixel_delta_v;

    auto pixel_sample = pixel_center + pixel_sample_square();
    auto ray_origin = center;
    auto ray_direction = pixel_sample - ray_origin;
    return ray(ray_origin, ray_direction);
  }
  // 在一个 square 像素中采样得到一个点
  vec3 pixel_sample_square() const {
    auto px = -0.5 + random_double();
    auto py = -0.5 + random_double();
    return px * pixel_delta_u + py * pixel_delta_v;
  }
};

#endif