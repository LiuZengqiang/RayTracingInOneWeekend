// 多个hittable的封装类
// 使用到了 智能指针 等特性
#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <memory>
#include <vector>

#include "hittable.h"

using std::make_shared;
using std::shared_ptr;

class hittable_list : public hittable {
 public:
  // 物体列表
  std::vector<shared_ptr<hittable>> objects;

  hittable_list() {}
  // 添加一个 物体
  hittable_list(shared_ptr<hittable> object) { add(object); }

  void clear() { objects.clear(); }

  void add(shared_ptr<hittable> object) { objects.push_back(object); }

  // 计算 光线 r 与物体列表第一个合法相交点
  bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = ray_t.max;

    for (const auto& object : objects) {
      if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
        hit_anything = true;
        closest_so_far = temp_rec.t;
        rec = temp_rec;
      }
    }
    return hit_anything;
  }
};

#endif