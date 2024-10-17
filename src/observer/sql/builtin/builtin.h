/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#pragma once

#include "common/value.h"
#include "common/utils.h"
#include <cmath>

namespace builtin {

extern RC length(const vector<Value> &args, Value &result);

extern RC round(const vector<Value> &args, Value &result);

extern RC year(const vector<Value> &args, Value &result);

extern RC month(const vector<Value> &args, Value &result);

extern RC day(const vector<Value> &args, Value &result);

extern RC date_format(const vector<Value> &args, Value &result);

extern RC distance(const vector<Value> &args, Value &result);

extern RC string_to_vector(const vector<Value> &args, Value &result);

extern RC vector_to_string(const vector<Value> &args, Value &result);

extern RC vector_dim(const vector<Value> &args, Value &result);

};  // namespace builtin