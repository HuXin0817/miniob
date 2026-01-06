/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "oblsm/table/ob_sstable.h"
#include "oblsm/util/ob_coding.h"
#include "common/log/log.h"
#include "common/lang/filesystem.h"
namespace oceanbase {

void ObSSTable::init()
{
  file_reader_ = make_unique<ObFileReader>(file_name_);
  RC rc        = file_reader_->open_file();
  ASSERT(OB_SUCC(rc), "OB_SUCC(rc)");

  uint32_t file_size       = file_reader_->file_size();
  string   block_size_data = file_reader_->read_pos(file_size - sizeof(uint32_t), sizeof(uint32_t));
  ASSERT(block_size_data.size() == sizeof(uint32_t), "block_size_data.size() == sizeof(uint32_t)");

  uint32_t block_size     = get_numeric<uint32_t>(block_size_data.data());
  uint32_t meta_data_size = file_size - block_size - sizeof(uint32_t);
  string   meta_data      = file_reader_->read_pos(block_size, meta_data_size);
  ASSERT(meta_data.size() == meta_data_size, "meta_data.size() == meta_data_size");

  uint32_t meta_number = get_numeric<uint32_t>(meta_data.data());
  size_t   offset      = sizeof(uint32_t);
  for (uint32_t i = 0; i < meta_number; i++) {
    uint32_t meta_size = get_numeric<uint32_t>(meta_data.data() + offset);
    offset += sizeof(uint32_t);

    BlockMeta meta;
    meta.decode(meta_data.substr(offset, meta_size));  // TODO: replace `substr` by string_view

    block_metas_.push_back(meta);
    offset += meta_size;
  }

  sst_id_ = get_numeric<uint32_t>(meta_data.data() + offset);
  offset += sizeof(uint32_t);
}

shared_ptr<ObBlock> ObSSTable::read_block_with_cache(uint32_t block_idx) const
{
  ASSERT(block_count() > 0, "block_count() > 0");
  ASSERT(block_idx < block_count(), "block_idx < block_count()");

  if (block_cache_ == nullptr) {
    return read_block(block_idx);
  }

  uint64_t hash = (static_cast<uint64_t>(sst_id_) << 32) | block_idx;
  shared_ptr<ObBlock> block = nullptr;
  if (block_cache_->get(hash, block)) {
    return block;
  }

  block = read_block(block_idx);
  block_cache_->put(hash, block);
  return block;
}

shared_ptr<ObBlock> ObSSTable::read_block(uint32_t block_idx) const
{
  ASSERT(block_count() > 0, "block_count() > 0");
  ASSERT(block_idx < block_count(), "block_idx < block_count()");

  auto   block = make_shared<ObBlock>(comparator_);
  string data  = file_reader_->read_pos(block_metas_[block_idx].offset_, block_metas_[block_idx].size_);
  block->decode(data);
  return block;
}

void ObSSTable::remove() { filesystem::remove(file_name_); }

ObLsmIterator *ObSSTable::new_iterator() { return new TableIterator(get_shared_ptr()); }

void TableIterator::read_block_with_cache()
{
  block_ = sst_->read_block_with_cache(curr_block_idx_);
  block_iterator_.reset(block_->new_iterator());
}

void TableIterator::seek_to_first()
{
  curr_block_idx_ = 0;
  read_block_with_cache();
  block_iterator_->seek_to_first();
}

void TableIterator::seek_to_last()
{
  curr_block_idx_ = block_cnt_ - 1;
  read_block_with_cache();
  block_iterator_->seek_to_last();
}

void TableIterator::next()
{
  block_iterator_->next();
  if (block_iterator_->valid()) {
  } else if (curr_block_idx_ < block_cnt_ - 1) {
    curr_block_idx_++;
    read_block_with_cache();
    block_iterator_->seek_to_first();
  }
}

void TableIterator::seek(const string_view &lookup_key)
{
  curr_block_idx_ = 0;
  // TODO: use binary search
  for (; curr_block_idx_ < block_cnt_; curr_block_idx_++) {
    const auto &block_meta = sst_->block_meta(curr_block_idx_);
    if (sst_->comparator()->compare(extract_user_key(block_meta.last_key_), extract_user_key_from_lookup_key(lookup_key)) >= 0) {
      break;
    }
  }
  if (curr_block_idx_ == block_cnt_) {
    block_iterator_ = nullptr;
    return;
  }
  read_block_with_cache();
  block_iterator_->seek(lookup_key);
};

}  // namespace oceanbase
