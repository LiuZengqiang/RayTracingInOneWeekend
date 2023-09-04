#ifndef INTERVAL_H
#define INTERVAL_H

class interval {
 public:
  double min, max;

  interval() : min(+infinity), max(-infinity) {}  // Default interval is empty

  interval(double _min, double _max) : min(_min), max(_max) {}
  // 判断 x 是否在目标范围内部(包含边界)
  bool contains(double x) const { return min <= x && x <= max; }
  // 判断 x 是否在目标范围内部(不包含边界)
  bool surrounds(double x) const { return min < x && x < max; }
  // 将 x clamp 到目标范围内部
  double clamp(double x) const {
    if (x < min) return min;
    if (x > max) return max;
    return x;
  }
  // 声明两个 静态常量
  static const interval empty, universe;
};
// 定义这两个静态常量，一个表示空，一个表示全局范围
const static interval empty(+infinity, -infinity);
const static interval universe(-infinity, +infinity);

#endif