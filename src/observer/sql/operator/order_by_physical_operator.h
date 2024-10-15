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

#include "sql/operator/physical_operator.h"
#include "sql/expr/tuple.h"
#include <functional>

class OrderByPhysicalOperator : public PhysicalOperator
{
public:
  OrderByPhysicalOperator(std::vector<OrderBySqlNode> order_by, std::vector<Expression *> exprs);

  virtual ~OrderByPhysicalOperator() = default;

  PhysicalOperatorType type() const override { return PhysicalOperatorType::ORDER_BY; }

  std::vector<OrderBySqlNode> &order_by() { return order_by_; }

  std::vector<Expression *> &exprs() { return exprs_; }

  RC fetch_and_sort_tables();
  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override;

private:
  std::vector<OrderBySqlNode> order_by_;

  /// 在 create order by stmt 之前提取 select clause 后的 field_expr (非agg_expr 中的) 和 agg_expr
  std::vector<Expression *> exprs_;

  SplicedTuple tuple_;

  using order_line = pair<vector<Value>, vector<Value>>;
  using order_func = std::function<bool(const order_line &, const order_line &)>;
  using order_list = std::priority_queue<order_line, vector<order_line>, order_func>;
  order_list order_and_field_line;
};
