/***************************************************************
 *                                                             *
 * @Author      : Koschei                                      *
 * @Email       : nitianzero@gmail.com                         *
 * @Date        : 2024/9/30                                    *
 * @Description : utils source file                            *
 *                                                             *
 * Copyright (c) 2024 Koschei                                  *
 * All rights reserved.                                        *
 *                                                             *
 ***************************************************************/

#include "utils.h"

#include <cstdio>
#include <cstdlib>

bool check_date(int y, int m, int d)
{
  // 定义每个月的天数
  static int mon[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  // 判断是否为闰年
  bool leap = (y % 400 == 0) || (y % 100 != 0 && y % 4 == 0);
  // 检查年份、月份和日期的合法性
  return (y > 0 && y <= 9999) && (m > 0 && m <= 12) &&
         (d > 0 && d <= (mon[m] + (m == 2 && leap ? 1 : 0)));  // 2月如果是闰年则加1天
}

RC parse_date(const char *str, int &result)
{
  int y, m, d;
  if (sscanf(str, "%d-%d-%d", &y, &m, &d) != 3) {
    return RC::INVALID_ARGUMENT;
  }
  if (!check_date(y, m, d)) {
    return RC::INVALID_ARGUMENT;
  }
  result = y * 10000 + m * 100 + d;
  return RC::SUCCESS;
}

RC parse_float_prefix(const char *str, float &result)
{
  char *end_ptr;
  // 输入不是有效的整数格式，会转换为 0.0
  double float_val = std::strtod(str, &end_ptr);
  // 注释以支持前缀解析
  // if (*end_ptr != '\0' && !isspace(*end_ptr)) {
  //   return RC::INVALID_ARGUMENT;  // 浮点数后应为结尾或空白字符
  // }
  // 默认认为 float_val 是否在 float 范围内
  result = static_cast<float>(float_val);
  return RC::SUCCESS;
}

// RC parse_vector_from_string(const char *str, std::vector<float> &vectorData) {
//   if (!str || *str != '[') {
//     return RC::INVALID_ARGUMENT;
//   }
//
//   std::string s = str;
//   // 去掉开头和结尾的方括号
//   if (s.back() != ']') {
//     return RC::INVALID_ARGUMENT;
//   }
//   s = s.substr(1, s.size() - 2);
//
//   std::stringstream ss(s);
//   std::string token;
//   float value;
//
//   while (std::getline(ss, token, ',')) {
//     std::stringstream valueStream(token);
//     if (!(valueStream >> value)) {
//       return RC::PARSE_ERROR;
//     }
//     vectorData.push_back(value);
//   }
//
//   return RC::SUCCESS;
// }