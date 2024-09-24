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
// Created by WangYunlai on 2022/07/01.
//

#pragma once

#include "sql/operator/physical_operator.h"

/**
 * @brief show index物理算子
 * @ingroup PhysicalOperator
 */
class ShowIndexPhysicalOperator : public PhysicalOperator
{
public:
  ShowIndexPhysicalOperator() {}

  virtual ~ShowIndexPhysicalOperator() = default;

  void add_expressions(std::vector<std::unique_ptr<Expression>> &&expressions) {}
  void add_projection(const Table *table, const IndexMeta *index);

  PhysicalOperatorType type() const override { return PhysicalOperatorType::SHOW_INDEX; }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  int cell_num() const { return tuple_.cell_num(); }

  Tuple *current_tuple() override;

private:
  ProjectTuple tuple_;
};