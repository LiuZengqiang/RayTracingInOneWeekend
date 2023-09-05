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
  metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}
  bool scatter(const ray& r_in, const hit_record& rec, color& attenuation,
               ray& scattered) const override {
    // 这是理想反射光线
    vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    // 将理想反射光线上加上一个 fuzzy
    scattered = ray(rec.p, reflected + fuzz * random_unit_vector());

    // scattered = ray(rec.p, reflected);
    attenuation = albedo;
    return (dot(scattered.direction(), rec.normal) > 0);
  }

 private:
  color albedo;  // 颜色衰减系数 attenuation
  double fuzz;   // 镜面反射光散射系数 fuzzy
};
// 新建一种具有折射属性的材料

class dielectric : public material {
 public:
  dielectric(double _etai_over_etat) : etai_over_etat(_etai_over_etat) {}

  bool scatter(const ray& r_in, const hit_record& rec, color& attenuation,
               ray& scattered) const override {
    // 先假设没有衰减进行测试
    attenuation = color(1.0, 1.0, 1.0);
    // 根据入射光的方向进行判断 eta/eta' 的值
    double refraction_ratio =
        rec.front_face ? (1.0 / etai_over_etat) : (etai_over_etat);

    // 先单位化入射光
    vec3 unit_direction = unit_vector(r_in.direction());

    // 判断入射光 theta 角是否很大，导致只发生反射，而没有折射
    double cos_theta = fmin(dot(rec.normal, -unit_direction), 1.0);

    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    bool cannot_refraction = refraction_ratio * sin_theta > 1.0;
    vec3 direction;

    if (cannot_refraction ||
        reflectance(cos_theta, refraction_ratio) > random_double()) {
      // 如果入射角度太大 或者 根据反射/折射比率计算得到该光线为反射->只有反射
      direction = reflect(unit_direction, rec.normal);
    } else {
      // 存在折射
      // 计算折射光线
      direction = refract(unit_direction, rec.normal, refraction_ratio);
    }
    scattered = ray(rec.p, direction);
    return true;
  }

 private:
  double etai_over_etat;

  // 计算 反射折射比率
  // 输入 入射光cos, 两者介质的折射率
  static double reflectance(double cosine, double ref_idx) {
    // 使用 Schlick's 逼近公式表示
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
  }
};

#endif