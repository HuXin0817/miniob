/***************************************************************
 *                                                             *
 * @Author      : Koschei                                      *
 * @Email       : nitianzero@gmail.com                         *
 * @Date        : 2024/10/12                                     *
 * @Description : Brief description of the file's purpose      *
 *                                                             *
 * Copyright (c) 2024 Koschei                                  *
 * All rights reserved.                                        *
 *                                                             *
 ***************************************************************/

#include "create_view_stmt.h"
#include "sql/stmt/create_table_stmt.h"
#include "event/sql_debug.h"

RC CreateViewStmt::create(Db *db, CreateViewSqlNode &create_view, Stmt *&stmt)
{
  SelectStmt *select_stmt = nullptr;
  if (create_view.create_view_select) {
    Stmt *create_view_select_stmt;
    RC    rc = SelectStmt::create(db, *create_view.create_view_select, create_view_select_stmt);
    if (OB_FAIL(rc)) {
      return rc;
    }
    select_stmt = dynamic_cast<SelectStmt *>(create_view_select_stmt);
    if (select_stmt == nullptr) {
      return RC::INTERNAL;
    }
  }

  stmt = new CreateViewStmt(create_view.relation_name, select_stmt);
  sql_debug("create view statement: view name %s", create_view.relation_name.c_str());
  return RC::SUCCESS;
}