/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "builtin.h"

namespace builtin {

RC length(const vector<Value> &args, Value &result)
{
  if (args.size() != 1) {
    return RC::INVALID_ARGUMENT;
  }
  if (args[0].attr_type() != AttrType::CHARS) {
    return RC::INVALID_ARGUMENT;
  }
  int length = static_cast<int>(args[0].to_string().size());
  result     = Value(length);
  return RC::SUCCESS;
}

RC round(const vector<Value> &args, Value &result)
{
  if (args.size() != 1 && args.size() != 2) {
    return RC::INVALID_ARGUMENT;
  }
  float number;
  int   decimals = 0;  // 默认四舍五入为整数
  if (args[0].attr_type() != AttrType::FLOATS) {
    return RC::INVALID_ARGUMENT;
  }
  if (args.size() == 2) {
    if (args[1].attr_type() != AttrType::INTS) {
      return RC::INVALID_ARGUMENT;
    }
    decimals = args[1].get_int();
  }
  number       = args[0].get_float();
  float factor = std::pow(10.f, static_cast<float>(decimals));
  float round  = std::round(number * factor) / factor;
  result       = Value(round);
  return RC::SUCCESS;
}

namespace date {
static string get_day_with_suffix(int day)
{
  if (day >= 11 && day <= 13) {
    return std::to_string(day) + "th";
  }
  switch (day % 10) {
    case 1: {
      return std::to_string(day) + "st";
    }
    case 2: {
      return std::to_string(day) + "nd";
    }
    case 3: {
      return std::to_string(day) + "rd";
    }
    default: {
      return std::to_string(day) + "th";
    }
  }
}

static string get_full_month_name(int month)
{
  switch (month) {
    case 1: return "January";
    case 2: return "February";
    case 3: return "March";
    case 4: return "April";
    case 5: return "May";
    case 6: return "June";
    case 7: return "July";
    case 8: return "August";
    case 9: return "September";
    case 10: return "October";
    case 11: return "November";
    case 12: return "December";
    default: return "";  // 如果月份值无效，返回一个错误字符串
  }
}

static RC get_year_month_day(const Value &value, int &year, int &month, int &day)
{
  if (value.attr_type() == AttrType::DATES) {
    // 提取年、月、日（假设日期格式为YYYYMMDD）
    int val = value.get_date();
    year    = val / 10000;        // 获取年份
    month   = (val / 100) % 100;  // 获取月份
    day     = val % 100;          // 获取日期
  }

  if (value.attr_type() == AttrType::CHARS) {
    // 日期格式假设为 '2019-9-17' 或 '2019-09-17'
    std::string date_str = value.to_string();
    if (sscanf(date_str.c_str(), "%d-%d-%d", &year, &month, &day) != 3) {
      return RC::INVALID_ARGUMENT;
    }
  }

  if (!check_date(year, month, day)) {
    return RC::ERROR_DATE;
  }

  return RC::SUCCESS;
}

}  // namespace date

RC year(const vector<Value> &args, Value &result)
{
  if (args.size() != 1) {
    return RC::INVALID_ARGUMENT;
  }
  if (args[0].attr_type() != AttrType::DATES && args[0].attr_type() != AttrType::CHARS) {
    return RC::INVALID_ARGUMENT;
  }
  int year, month, day;
  RC  rc = date::get_year_month_day(args[0], year, month, day);
  if (OB_FAIL(rc)) {
    return rc;
  }
  result = Value(year);
  return RC::SUCCESS;
}

RC month(const vector<Value> &args, Value &result)
{
  if (args.size() != 1) {
    return RC::INVALID_ARGUMENT;
  }
  if (args[0].attr_type() != AttrType::DATES && args[0].attr_type() != AttrType::CHARS) {
    return RC::INVALID_ARGUMENT;
  }
  int year, month, day;
  RC  rc = date::get_year_month_day(args[0], year, month, day);
  if (OB_FAIL(rc)) {
    return rc;
  }
  result = Value(month);
  return RC::SUCCESS;
}

RC day(const vector<Value> &args, Value &result)
{
  if (args.size() != 1) {
    return RC::INVALID_ARGUMENT;
  }
  if (args[0].attr_type() != AttrType::DATES && args[0].attr_type() != AttrType::CHARS) {
    return RC::INVALID_ARGUMENT;
  }
  int year, month, day;
  RC  rc = date::get_year_month_day(args[0], year, month, day);
  if (OB_FAIL(rc)) {
    return rc;
  }
  result = Value(day);
  return RC::SUCCESS;
}

RC date_format(const vector<Value> &args, Value &result)
{
  if (args.size() != 2) {
    return RC::INVALID_ARGUMENT;
  }
  if (args[0].attr_type() != AttrType::DATES && args[0].attr_type() != AttrType::CHARS) {
    return RC::INVALID_ARGUMENT;
  }
  if (args[1].attr_type() != AttrType::CHARS) {
    return RC::INVALID_ARGUMENT;
  }

  int year, month, day;
  RC  rc = date::get_year_month_day(args[0], year, month, day);
  if (OB_FAIL(rc)) {
    return rc;
  }

  auto fmt = args[1].to_string();

  string str;

  // 遍历格式字符串，并替换格式符
  for (size_t i = 0; i < fmt.length(); ++i) {
    if (fmt[i] == '%' && i + 1 < fmt.length()) {
      switch (fmt[i + 1]) {
        case 'Y':  // 四位数年份
          str += std::to_string(year);
          break;
        case 'y':  // 两位数年份
          str += std::to_string(year).substr(2, 2);
          break;
        case 'm':  // 两位数月份
          str += (month < 10 ? "0" : "") + std::to_string(month);
          break;
        case 'c':  // 不带前导零的月份
          str += std::to_string(month);
          break;
        case 'M':  // 完整的月份名称
          str += date::get_full_month_name(month);
          break;
        case 'd':  // 两位数日期
          str += (day < 10 ? "0" : "") + std::to_string(day);
          break;
        case 'e':  // 不带前导零的日期
          str += std::to_string(day);
          break;
        case 'D':  // 带序数后缀的日期
          str += date::get_day_with_suffix(day);
          break;
        default:  // 未知格式符，按原样输出
          str += fmt[i + 1];
          break;
      }
      ++i;  // 跳过格式符的下一个字符
    } else {
      str += fmt[i];  // 普通字符直接追加
    }
  }

  result = Value(str.c_str());
  return RC::SUCCESS;
}

RC distance(const vector<Value> &args, Value &result)
{
  if (args.size() != 2) {
    return RC::INVALID_ARGUMENT;
  }
  if (args[0].attr_type() != AttrType::VECTORS && args[0].attr_type() != AttrType::CHARS) {
    return RC::INVALID_ARGUMENT;
  }
  if (args[1].attr_type() != AttrType::VECTORS && args[1].attr_type() != AttrType::CHARS) {
    return RC::INVALID_ARGUMENT;
  }

  Value value0, value1;
  if (args[0].attr_type() == AttrType::CHARS) {
    RC rc = Value::cast_to(args[0], AttrType::VECTORS, value0);
    if (OB_FAIL(rc)) {
      return rc;
    }
  } else if (args[0].attr_type() == AttrType::VECTORS) {
    value0 = args[0];
  } else {
    return RC::INVALID_ARGUMENT;
  }

  if (args[1].attr_type() == AttrType::CHARS) {
    RC rc = Value::cast_to(args[1], AttrType::VECTORS, value1);
    if (OB_FAIL(rc)) {
      return rc;
    }
  } else if (args[1].attr_type() == AttrType::VECTORS) {
    value1 = args[1];
  } else {
    return RC::INVALID_ARGUMENT;
  }

  auto v0_length = value0.get_vector_length();
  auto v1_length = value1.get_vector_length();
  if (v0_length != v1_length) {
    return RC::VECTOR_LENGTG_ARE_INCONSISTENT;
  }

  float ans = 0.0;
  for (int i = 0; i < v0_length; i++) {
    float v0 = value0.get_vector_element(i);
    float v1 = value1.get_vector_element(i);
    ans += (v0 - v1) * (v0 - v1);
  }
  ans    = std::sqrt(ans);
  result = Value(ans);
  return RC::SUCCESS;
}

RC string_to_vector(const vector<Value> &args, Value &result)
{
  if (args.size() != 1) {
    return RC::INVALID_ARGUMENT;
  }
  if (args[0].attr_type() != AttrType::CHARS) {
    return RC::INVALID_ARGUMENT;
  }
  return Value::cast_to(args[0], AttrType::VECTORS, result);
}

RC vector_to_string(const vector<Value> &args, Value &result)
{
  if (args.size() != 1) {
    return RC::INVALID_ARGUMENT;
  }
  if (args[0].attr_type() != AttrType::VECTORS) {
    return RC::INVALID_ARGUMENT;
  }
  return Value::cast_to(args[0], AttrType::CHARS, result);
}

RC vector_dim(const vector<Value> &args, Value &result)
{
  if (args.size() != 1) {
    return RC::INVALID_ARGUMENT;
  }
  Value value;
  if (args[0].attr_type() == AttrType::CHARS) {
    RC rc = Value::cast_to(args[0], AttrType::VECTORS, value);
    if (OB_FAIL(rc)) {
      return rc;
    }
  } else if (args[0].attr_type() == AttrType::VECTORS) {
    value = args[0];
  } else {
    return RC::INVALID_ARGUMENT;
  }

  result = Value(value.get_vector_length());
  return RC::SUCCESS;
}

}  // namespace builtin