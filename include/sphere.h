/**
 * @file sphere.h
 * @author Liuzengqiang (12021032@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2023-09-06
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "material.h"
#include "vec3.h"
/**
 * @brief 球类
 *
 *
 */
class sphere : public hittable {
 public:
  sphere(point3 _center, double _radius, shared_ptr<material> _material)
      : center(_center), radius(_radius), mat(_material) {}

  /**
   * @brief 根据 球与射线的相交公式 计算光线是否与球相交, 详细公式推导过程见
   * [Ray Tracing in One Weekend - 5.Adding a Sphere]
   * (https://raytracing.github.io/books/RayTracingInOneWeekend.html#addingasphere/creatingourfirstraytracedimage)
   * 如有合法交点则返回true, 否则返回false
   *
   * @param r 入射光线
   * @param ray_t 入射光(射线)的合法距离
   * @param rec 如果有合法的交点, 则在rec内记录交点信息
   * @return true
   * @return false
   */
  bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    // 如果discriminant<0 说明没有交点
    if (discriminant < 0) return false;

    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
      // 如果较小的解不在合法范围内 那就尝试使用较大的那个解 root =
      // (-half_b+sqrtd)/a;
      root = (-half_b + sqrtd) / a;
      if (!ray_t.surrounds(root)) {
        // 如果较大的那个节也不在合法范围内，说明没有合法交点
        return false;
      }
    }
    // 更新交点 rec
    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat = mat;

    return true;
  }

 private:
  point3 center;             // 球的中点
  double radius;             // 球的半径
  shared_ptr<material> mat;  // 球的表面材料
};

#endif