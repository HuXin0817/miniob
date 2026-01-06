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

#include "common/lang/atomic.h"
#include "common/lang/string.h"
#include "common/lang/memory.h"

namespace oceanbase {

/**
 * @class ObBloomfilter
 * @brief A simple Bloom filter implementation(Need to support concurrency).
 */
class ObBloomfilter
{
public:
  /**
   * @brief Constructs a Bloom filter with specified parameters.
   *
   * @param hash_func_count Number of hash functions to use. Default is 4.
   * @param totoal_bits Total number of bits in the Bloom filter. Default is 65536.
   */
  ObBloomfilter(size_t hash_func_count = 4, size_t totoal_bits = 65536)
      : hash_func_count_(hash_func_count), total_bits_(totoal_bits), object_count_(0)
  {
    if (total_bits_ == 0) {
      total_bits_ = 1;
    }
    word_count_ = (total_bits_ + 63) / 64;  // ceil(total_bits / 64)
    bit_array_  = make_unique<atomic<uint64_t>[]>(word_count_);
    for (size_t i = 0; i < word_count_; i++) {
      bit_array_[i].store(0, std::memory_order_relaxed);
    }
  }

  /**
   * @brief Inserts an object into the Bloom filter.
   * @details This method computes hash values for the given object and sets corresponding bits in the filter.
   * @param object The object to be inserted.
   */
  void insert(const string &object)
  {
    uint64_t h1 = std::hash<string>()(object);
    uint64_t h2 = h1 + 1;  // avoid zero step; ensure h2 != 0

    for (size_t i = 0; i < hash_func_count_; ++i) {
      uint64_t combined_hash = h1 + i * h2;
      uint64_t index         = combined_hash % total_bits_;

      size_t   word_index = index / 64;
      size_t   bit_offset = index % 64;
      uint64_t mask       = 1ULL << bit_offset;

      bit_array_[word_index].fetch_or(mask, std::memory_order_relaxed);
    }

    // Atomic increment of object count
    object_count_.fetch_add(1, std::memory_order_relaxed);
  }

  /**
   * @brief Clears all entries in the Bloom filter.
   *
   * @details Resets the filter, removing all previously inserted objects.
   */
  void clear()
  {
    for (size_t i = 0; i < word_count_; i++) {
      bit_array_[i].store(0, std::memory_order_relaxed);
    }
    object_count_.store(0, std::memory_order_relaxed);
  }

  /**
   * @brief Checks if an object is possibly in the Bloom filter.
   *
   * @param object The object to be checked.
   * @return true if the object might be in the filter, false if definitely not.
   */
  bool contains(const string &object) const
  {
    uint64_t h1 = std::hash<string>()(object);
    uint64_t h2 = h1 + 1;

    for (size_t i = 0; i < hash_func_count_; ++i) {
      uint64_t combined_hash = h1 + i * h2;
      uint64_t index         = combined_hash % total_bits_;

      size_t   word_index = index / 64;
      size_t   bit_offset = index % 64;
      uint64_t mask       = 1ULL << bit_offset;

      if ((bit_array_[word_index].load(std::memory_order_relaxed) & mask) == 0) {
        return false;
      }
    }
    return true;  // possibly present
  }

  /**
   * @brief Returns the count of objects inserted into the Bloom filter.
   */
  size_t object_count() const { return object_count_.load(std::memory_order_relaxed); }

  /**
   * @brief Checks if the Bloom filter is empty.
   * @return true if the filter is empty, false otherwise.
   */
  bool empty() const { return 0 == object_count(); }

private:
  size_t                         hash_func_count_;
  size_t                         total_bits_;
  size_t                         word_count_;
  unique_ptr<atomic<uint64_t>[]> bit_array_;
  mutable atomic<size_t>         object_count_;
};

}  // namespace oceanbase
