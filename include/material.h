// 材料属性 类
#ifndef MATERIAL_H
#define MATERIAL_H
#include "color.h"
#include "hittable.h"
#include "rtweekend.h"

class hit_record;
// 这里只是声明，没有实现
class material {
 public:
  virtual ~material() = default;
  // 描述该属性的材料 散射光线 的行为
  // 指定：入射光线 r_in, 交点 rec, 衰减 attenuation, 反射光线 scattered
  virtual bool scatter(const ray& r_in, const hit_record& rec,
                       color& attenuation, ray& scattered) const = 0;
};

// 新建 Lambertian 材料属性
class lambertian : public material {
 public:
  lambertian(const color& a) : albedo(a) {}
  // lambertian 会在表面法向周围的半球内 散射，并且存在一定的衰减attenuation
  bool scatter(const ray& r_in, const hit_record& rec, color& attenuation,
               ray& scattered) const override {
    // 这里有可能 等于零向量，因此需要进行判断
    vec3 scatter_direction = rec.normal + random_unit_vector();
    if (scatter_direction.near_zero()) {
      scatter_direction = rec.normal;
    }

    scattered = ray(rec.p, scatter_direction);
    attenuation = albedo;
    return true;
  }

 private:
  color albedo;
};
// 新建一种 金属材料
class metal : public material {
 public:
  metal(const color& a) : albedo(a) {}
  bool scatter(const ray& r_in, const hit_record& rec, color& attenuation,
               ray& scattered) const override {
    vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    scattered = ray(rec.p, reflected);
    attenuation = albedo;
    return true;
  }

 private:
  color albedo;
};

#endif