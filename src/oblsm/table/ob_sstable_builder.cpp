/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "oblsm/table/ob_sstable_builder.h"
#include "oblsm/util/ob_coding.h"

namespace oceanbase {

// TODO: refactor build with mem_table/iterator logic.
RC ObSSTableBuilder::build(shared_ptr<ObMemTable> mem_table, const std::string &file_name, uint32_t sst_id)
{
  file_writer_ = make_unique<ObFileWriter>(file_name);
  if (RC rc = file_writer_->open_file(); OB_FAIL(rc)) {
    return rc;
  }

  auto mem_table_iter = mem_table->new_iterator();
  for (mem_table_iter->seek_to_first(); mem_table_iter->valid();) {
    auto key = mem_table_iter->key();
    ASSERT(!key.empty(), "!key.empty()");
    if (curr_blk_first_key_.empty()) {
      curr_blk_first_key_ = key;
    }

    if (RC rc = block_builder_.add(key, mem_table_iter->value()); rc == RC::FULL) {
      finish_build_block();
      curr_blk_first_key_.clear();
    } else if (rc != RC::SUCCESS) {
      // block_builder_ error
      delete mem_table_iter;
      reset();
      return rc;
    } else {
      mem_table_iter->next();
    }
  }
  delete mem_table_iter;

  if (!curr_blk_first_key_.empty()) {
    finish_build_block();
    curr_blk_first_key_.clear();
  }

  string meta_contents;
  put_numeric<uint32_t>(&meta_contents, block_metas_.size());
  for (auto &meta : block_metas_) {
    auto meta_content = meta.encode();
    put_numeric<uint32_t>(&meta_contents, meta_content.size());
    meta_contents.append(meta_content);
  }

  put_numeric<uint32_t>(&meta_contents, sst_id);
  put_numeric<uint32_t>(&meta_contents, curr_offset_);
  file_writer_->write(meta_contents);
  curr_offset_ += meta_contents.size();

  file_writer_->flush();
  file_writer_->close_file();

  return RC::SUCCESS;
}

void ObSSTableBuilder::finish_build_block()
{
  string      last_key       = block_builder_.last_key();
  string_view block_contents = block_builder_.finish();
  file_writer_->write(block_contents);
  block_metas_.push_back(BlockMeta(curr_blk_first_key_, last_key, curr_offset_, block_contents.size()));
  // TODO: block aligned to BLOCK_SIZE
  curr_offset_ += block_contents.size();
  block_builder_.reset();
}

shared_ptr<ObSSTable> ObSSTableBuilder::get_built_table()
{
  // TODO: sstable should have more metadata
  shared_ptr<ObSSTable> sstable = make_shared<ObSSTable>(sst_id_, file_writer_->file_name(), comparator_, block_cache_);
  sstable->init();
  return sstable;
}

void ObSSTableBuilder::reset()
{
  block_builder_.reset();
  curr_blk_first_key_.clear();
  if (file_writer_ != nullptr) {
    file_writer_.reset(nullptr);
  }
  block_metas_.clear();
  curr_offset_ = 0;
  sst_id_      = 0;
  file_size_   = 0;
}
}  // namespace oceanbase
