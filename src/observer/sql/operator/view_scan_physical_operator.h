/***************************************************************
 *                                                             *
 * @Author      : Koschei                                      *
 * @Email       : nitianzero@gmail.com                         *
 * @Date        : 2024/10/13                                     *
 * @Description : Brief description of the file's purpose      *
 *                                                             *
 * Copyright (c) 2024 Koschei                                  *
 * All rights reserved.                                        *
 *                                                             *
 ***************************************************************/

#pragma once

#include "common/rc.h"
#include "sql/operator/physical_operator.h"
#include "storage/record/record_manager.h"
#include "common/types.h"
#include "sql/expr/expression_tuple.h"
#include "storage/table/view.h"

class View;

/**
 * @brief 视图扫描物理算子
 * @ingroup PhysicalOperator
 */
class ViewScanPhysicalOperator : public PhysicalOperator
{
public:
  ViewScanPhysicalOperator(View *view, ReadWriteMode mode) : view_(view) { select_expr_ = view_->select_oper().get(); }

  ~ViewScanPhysicalOperator() override = default;

  std::string param() const override;

  PhysicalOperatorType type() const override { return PhysicalOperatorType::VIEW_SCAN; }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override;

  void set_predicates(std::vector<std::unique_ptr<Expression>> &&exprs);

private:
  RC filter(RowTuple &tuple, bool &result);

  // 把查询的结果转为当前视图的记录，就相当于从视图表中取出来一个记录
  RC next_record();

private:
  View                                    *view_ = nullptr;
  Trx                                     *trx_  = nullptr;
  PhysicalOperator                        *select_expr_;
  Record                                   current_record_;
  RowTuple                                 tuple_;
  std::vector<std::unique_ptr<Expression>> predicates_;  // TODO chang predicate to table tuple filter
};
