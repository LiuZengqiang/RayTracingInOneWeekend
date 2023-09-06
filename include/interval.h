/**
 * @file interval.h
 * @author Liuzengqiang (12021032@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2023-09-06
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef INTERVAL_H
#define INTERVAL_H
/**
 * @brief 范围类, 用于规范,限制光线的合法区间.
 * 例如：用于判断光线(射线) r(t) 中的t参数是否合法
 *
 */
class interval {
 public:
  double min, max;
  interval() : min(+infinity), max(-infinity) {}  // Default interval is empty

  interval(double _min, double _max) : min(_min), max(_max) {}
  // 判断 x 是否在目标范围内部(包含边界)
  bool contains(double x) const { return min <= x && x <= max; }
  // 判断 x 是否在目标范围内部(不包含边界)
  bool surrounds(double x) const { return min < x && x < max; }
  // 将 x 夹到目标范围内部
  double clamp(double x) const {
    if (x < min) return min;
    if (x > max) return max;
    return x;
  }

  static const interval empty, universe;
};
// 全局静态范围, empty 空范围, universe 正负无限范围
const static interval empty(+infinity, -infinity);
const static interval universe(-infinity, +infinity);

#endif