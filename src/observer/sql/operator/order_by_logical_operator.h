/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by HuXin on 24-10-9.
//

#pragma once

#include "sql/operator/logical_operator.h"

/**
 * @brief 逻辑算子
 * @ingroup LogicalOperator
 */
class OrderByLogicalOperator : public LogicalOperator
{
public:
  OrderByLogicalOperator(std::vector<OrderBySqlNode> order_by, const std::vector<Expression *> exprs)
      : order_by_(std::move(order_by)), exprs_(std::move(exprs))
  {}

  LogicalOperatorType type() const override { return LogicalOperatorType::ORDER_BY; }

  std::vector<OrderBySqlNode> &order_by() { return order_by_; }

  std::vector<Expression *> &exprs() { return exprs_; }

private:
  std::vector<OrderBySqlNode> order_by_;

  /// 在 create order by stmt 之前提取 select clause 后的 field_expr (非agg_expr 中的) 和 agg_expr
  std::vector<Expression *> exprs_;
};