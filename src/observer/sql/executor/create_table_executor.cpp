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
// Created by Wangyunlai on 2023/6/13.
//

#include "sql/executor/create_table_executor.h"

#include "common/log/log.h"
#include "event/session_event.h"
#include "event/sql_event.h"
#include "session/session.h"
#include "sql/stmt/create_table_stmt.h"
#include "storage/db/db.h"
#include "sql/optimizer/logical_plan_generator.h"
#include "sql/operator/logical_operator.h"
#include "sql/operator/project_logical_operator.h"
#include "sql/optimizer/physical_plan_generator.h"
#include "storage/trx/trx.h"

RC CreateTableExecutor::execute(SQLStageEvent *sql_event)
{
  RC       rc;
  Stmt    *stmt    = sql_event->stmt();
  Session *session = sql_event->session_event()->session();
  ASSERT(stmt->type() == StmtType::CREATE_TABLE,
      "create table executor can not run this command: %d",
      static_cast<int>(stmt->type()));

  CreateTableStmt *create_table_stmt = static_cast<CreateTableStmt *>(stmt);
  const char      *table_name        = create_table_stmt->table_name().c_str();
  if (create_table_stmt->create_table_select_stmt()) {
    std::vector<FieldExpr *>   field_exprs;
    std::unordered_set<string> table_names;
    for (auto &expr : create_table_stmt->create_table_select_stmt()->query_expressions()) {
      auto field_expr = dynamic_cast<FieldExpr *>(expr.get());
      if (field_expr) {
        table_names.emplace(field_expr->table_name());
        field_exprs.push_back(field_expr);
      } else {
        return RC::INTERNAL;
      }
    }

    std::vector<AttrInfoSqlNode> attr_infos = create_table_stmt->attr_infos();
    if (attr_infos.empty()) {
      for (auto &expr : field_exprs) {
        AttrInfoSqlNode attr_info;
        if (table_names.size() == 1) {
          attr_info.name = expr->field_name();
        } else {
          attr_info.name = expr->table_name();
          attr_info.name += ".";
          attr_info.name += expr->field_name();
        }
        attr_info.length   = expr->field().meta()->len();
        attr_info.type     = expr->field().meta()->type();
        attr_info.nullable = expr->field().meta()->nullable();

        attr_infos.push_back(attr_info);
      }
    }

    rc = session->get_current_db()->create_table(table_name, attr_infos, create_table_stmt->storage_format());
    if (OB_FAIL(rc)) {
      return rc;
    }

    // TODO: insert records
    unique_ptr<LogicalOperator> logical_oper = nullptr;
    LogicalPlanGenerator::create(create_table_stmt->create_table_select_stmt(), logical_oper);
    if (!logical_oper) {
      return RC::INTERNAL;
    }

    unique_ptr<PhysicalOperator> physical_oper = nullptr;
    PhysicalPlanGenerator::create(*logical_oper, physical_oper);
    auto table_ = session->get_current_db()->find_table(table_name);
    if (!physical_oper) {
      return RC::INTERNAL;
    }

    physical_oper->open(session->current_trx());
    while (RC::SUCCESS == (rc = physical_oper->next())) {
      auto          tuple = physical_oper->current_tuple();
      int           num   = tuple->cell_num();
      vector<Value> values;
      for (int i = 0; i < num; i++) {
        Value cell;
        rc = tuple->cell_at(i, cell);
        if (OB_FAIL(rc)) {
          return rc;
        }
        values.push_back(cell);
      }

      Record record;
      rc = table_->make_record(static_cast<int>(values.size()), values.data(), record);
      if (OB_FAIL(rc)) {
        return rc;
      }

      rc = session->current_trx()->insert_record(table_, record);
      if (OB_FAIL(rc)) {
        return rc;
      }
    }

    rc = physical_oper->close();
    if (OB_FAIL(rc)) {
      return rc;
    }

    rc = RC::SUCCESS;
  } else {
    rc = session->get_current_db()->create_table(
        table_name, create_table_stmt->attr_infos(), create_table_stmt->storage_format());
  }

  return rc;
}