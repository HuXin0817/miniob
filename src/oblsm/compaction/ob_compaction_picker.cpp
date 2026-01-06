/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "oblsm/compaction/ob_compaction_picker.h"
#include "common/log/log.h"

namespace oceanbase {

// TODO: put it in options
unique_ptr<ObCompaction> TiredCompactionPicker::pick(SSTablesPtr sstables)
{
  if (sstables->size() < options_->default_run_num) {
    return nullptr;
  }
  unique_ptr<ObCompaction> compaction(new ObCompaction(0));
  // TODO(opt): a tricky compaction picker, just pick all sstables if enough sstables.
  for (size_t i = 0; i < sstables->size(); ++i) {
    size_t tire_i_size = (*sstables)[i].size();
    for (size_t j = 0; j < tire_i_size; ++j) {
      compaction->inputs_[0].emplace_back((*sstables)[i][j]);
    }
  }
  // TODO: LOG_DEBUG for debug
  return compaction;
}

vector<shared_ptr<ObSSTable>> LeveledCompactionPicker::find_overlap_tables(
    const shared_ptr<ObSSTable> &target, const vector<shared_ptr<ObSSTable>> &tables)
{
  auto key_range_overlap = [this](const string &a_min, const string &a_max, const string &b_min, const string &b_max) {
    return !(comp.compare(a_max, b_min) < 0 || comp.compare(b_max, a_min) < 0);
  };

  vector<shared_ptr<ObSSTable>> result;
  for (auto &sst : tables) {
    if (key_range_overlap(sst->first_key(), sst->last_key(), target->first_key(), target->last_key())) {
      result.push_back(sst);
    } else if (result.size()) {
      break;
    }
  }
  return result;
}

unique_ptr<ObCompaction> LeveledCompactionPicker::pick(SSTablesPtr sstables)
{
  size_t num_levels = sstables->size();

  if ((*sstables)[0].size() >= options_->default_l0_file_num) {
    unique_ptr<ObCompaction> compaction(new ObCompaction(0));
    compaction->level_     = 0;
    compaction->inputs_[0] = {(*sstables)[0][0]};
    compaction->inputs_[1] = find_overlap_tables((*sstables)[0][0], (*sstables)[1]);
    return compaction;
  }

  for (size_t level = 1; level < num_levels - 1; ++level) {
    size_t total_size = 0;
    for (const auto &sst : (*sstables)[level]) {
      total_size += sst->size();
    }

    if (total_size >= max_byte(level)) {
      if (!(*sstables)[level].empty()) {
        const auto &select_sst = select_compaction_tables((*sstables)[level]);

        unique_ptr<ObCompaction> compaction(new ObCompaction(level));
        compaction->inputs_[0] = {select_sst};
        compaction->inputs_[1] = find_overlap_tables(select_sst, (*sstables)[level + 1]);
        return compaction;
      }
    }
  }

  return nullptr;
}

ObCompactionPicker *ObCompactionPicker::create(CompactionType type, ObLsmOptions *options)
{

  switch (type) {
    case CompactionType::TIRED: return new TiredCompactionPicker(options);
    case CompactionType::LEVELED: return new LeveledCompactionPicker(options);
    default: return nullptr;
  }
  return nullptr;
}

}  // namespace oceanbase