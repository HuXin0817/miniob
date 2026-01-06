/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
   miniob is licensed under Mulan PSL v2.
   You can use this software according to the terms and conditions of the Mulan PSL v2.
   You may obtain a copy of Mulan PSL v2 at:
            http://license.coscl.org.cn/MulanPSL2
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
   EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
   MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
   See the Mulan PSL v2 for more details. */

#include "oblsm/wal/ob_lsm_wal.h"
#include "common/log/log.h"
#include "oblsm/util/ob_coding.h"
#include "oblsm/util/ob_file_reader.h"

namespace oceanbase {
RC WAL::recover(const std::string &wal_file, std::vector<WalRecord> &wal_records)
{
  wal_records.clear();
  if (RC rc = open(wal_file); OB_FAIL(rc)) {
    return rc;
  }
  ObFileReader reader(wal_file);
  if (RC rc = reader.open_file(); OB_FAIL(rc)) {
    return rc;
  }
  string       buffer = reader.read_pos(0, reader.file_size());
  size_t       offset = 0;
  while (offset < buffer.size()) {
    uint64_t seq = get_numeric<uint64_t>(buffer.data() + offset);
    offset += sizeof(uint64_t);
    auto key = get_length_prefixed_string(buffer.data() + offset);
    offset += sizeof(size_t) + key.size();
    auto val = get_length_prefixed_string(buffer.data() + offset);
    offset += sizeof(size_t) + val.size();
    wal_records.emplace_back(seq, string(key), string(val));
  }
  return RC::SUCCESS;
}

RC WAL::put(uint64_t seq, string_view key, string_view val)
{
  ASSERT(file_writer_, "file_writer_");
  RC rc = RC::SUCCESS;
  put_numeric<uint64_t>(&buffer_, seq);
  put_numeric<size_t>(&buffer_, key.size());
  buffer_.append(key.data(), key.size());
  put_numeric<size_t>(&buffer_, val.size());
  buffer_.append(val.data(), val.size());
  if (buffer_.size() >= MAX_LOG_BUFFER_SIZE) {
    rc = sync();
  }
  return rc;
}
}  // namespace oceanbase