/**
 * @file hittable.h
 * @author Liuzengqiang (12021032@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2023-09-06
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "rtweekend.h"

class material;
/**
 * @brief 记录光线与物体的交点信息
 *
 */
class hit_record {
 public:
  point3 p;  // 交点的坐标
  // 交点处的法向, 与入射光方向相反(可能指向物体外也可能指向物体内)
  vec3 normal;
  shared_ptr<material> mat;  // 交点处材料属性
  double t;                  // 光线的传播距离
  bool front_face;           // 该交点是否是物体的外表面
  /**
   * @brief 设置该交点是否是物体的外表面
   *
   * @param r 入射光
   * @param outward_normal 物体的外表面法向(向外)
   */
  void set_face_normal(const ray& r, const vec3& outward_normal) {
    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? (outward_normal) : (-outward_normal);
  }
};
/**
 * @brief 可与光线相交的类, 所有可与光线作用的物体都必须继承该类并实现其中的
 * hit() 函数
 *
 */
class hittable {
 public:
  // =default 关键字令编译器自动生成默认的构造函数
  virtual ~hittable() = default;
  virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif