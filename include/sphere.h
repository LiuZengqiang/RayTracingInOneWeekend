// sphere类 该类继承 hittable
#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "material.h"
#include "vec3.h"
class sphere : public hittable {
 public:
  // 构造函数包含 球心、半径
  sphere(point3 _center, double _radius, shared_ptr<material> _material)
      : center(_center), radius(_radius), mat(_material) {}

  // 计算光线是否与该球相交
  // ray_tmin 和 ray_tmax 给出合法光线的距离约束
  // 返回值bool表示是否有合法的相交点
  // 引用参数rec表示 如果有合法相交，合法的相交点为 rec
  bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
    // 光线与球的相交计算公式，求解交点t
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
    // 这个 root 就是 t
    // root = (-half_b-sqrtd)/a; 是较小的那个解
    if (!ray_t.surrounds(root)) {
      // 如果 较小的解不在合法范围内[ray_tmin, ray_tmax]
      // 那就尝试使用较大的那个解 root = (-half_b+sqrtd)/a;
      root = (-half_b + sqrtd) / a;
      if (!ray_t.surrounds(root)) {
        // 如果较大的那个节也不在合法范围内，说明没有合法交点
        return false;
      }
    }
    // 不然，root = (-half_b-sqrtd)/a; 就是合法的解
    // 那么交点的rec.t就等于root
    // 位置rec.p等于ray.at(root)
    // 法向等于球面(rec.p)处的法向
    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    // 更新交点的材料属性
    rec.mat = mat;

    return true;
  }

 private:
  point3 center;
  double radius;
  shared_ptr<material> mat;  // 球的表面材料属性
};

#endif