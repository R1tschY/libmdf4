/*
 * file.cpp
 *
 *  This file is part of libmdf4.
 *
 *  Copyright (C) 2014 Richard Liebscher <r1tschy@yahoo.de>
 *
 *  libmdf4 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libmdf4 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "file.h"

#include <cstring>
#include <cassert>
#include <boost/optional.hpp>

#include "detail/mdf4.h"

namespace mdf {

void file::open(const char* filename) {
  handle_ = std::make_shared<rawfile>();
  handle_->open(filename, "r");

  prase_idblock();
  prase_basic_hdblock();
  prase_groups();
}

void file::prase_idblock() {
  // prase IDBLOCK
  // not needed (hopefully): handle_.seek(64, rawfile::seek_orgin::begin);
  assert(handle_->tell() == 0);

  idblock id;
  handle_->read(id);

  if (!std::equal(id.file_id, id.file_id + 8, "MDF     ")) {
    std::string e(id.file_id, 8);
    printf("<<%s\n", e.c_str());
    throw error("format error: not a mdf file"); // not a mdf file
  }

  if (!isdigit(id.format_id[0]) || id.format_id[1] != '.' ||
      !isdigit(id.format_id[2]) || !isdigit(id.format_id[3]) ||
      id.format_id[4] != ' ' || id.format_id[5] != ' ' ||
      id.format_id[6] != ' ' || id.format_id[7] != ' ') {
    // file is corrupted
    throw error("file is corrupted");
  }
  file_version_ = static_cast<uint16_t>(
      (id.format_id[0]-'0') * 100 +
      (id.format_id[2]-'0') * 10 + 
      (id.format_id[3] - '0'));

  /*for 3.x: byte_order_ = (id.default_byte_order == 0) ?
      byte_order::little_endian :
      byte_order::big_endian;
  if (byte_order_ == byte_order::big_endian) {
    // big endian not implemented
    handle_.close();
    return false;
  }

  if (id.default_floating_point_format != 0) {
    // only IEEE 754 double implemented
    return false;
  }*/

  if (file_version_ != id.version_number) {
    // file is corrupted
    throw error("format error: file mdf versions not matching");
  }

  if (file_version_  <= 300 || file_version_  >= 500) {
    // only 4.x implemented
    throw error("only mdf 4.x implemented");
  }

  /*for 3.x:
  if (id.code_page_number != 0) {
    // warning: may incorrect strings
  }*/
}

std::string file::get_mdf_version_string() const {
  int mj = file_version_ / 100;
  int _t =  file_version_ % 100;
  int mi = _t / 10;
  int m = _t % 10;

  std::string result;
  result.push_back(static_cast<char>(mj + '0'));
  result.push_back('.');
  result.push_back(static_cast<char>(mi + '0'));
  result.push_back(static_cast<char>(m + '0'));
  return result;
}

std::string file::get_generator_name() const {
  handle_->seek(0, rawfile::seek_orgin::begin);

  idblock id;
  handle_->read(id);

  std::string result(8, '\0');
  std::copy(id.program_id, id.program_id + 8, result.begin());
  return result;
}

void file::prase_basic_hdblock() {
  // prase HDBLOCK
  handle_->seek(64, rawfile::seek_orgin::begin);

  // read header
  block_header header = prase_block_header(handle_.get(), make_id('H', 'D'));

  // read links
  handle_->read_to_container(links_, header.link_count);
}

std::string file::get_metadata_comment() const {
  return prase_tx(handle_.get(), links_[5]);
}

std::string file::get_comment() const {
  return extract_tx_from_xml(get_metadata_comment());
}

void file::prase_groups() {
  // for all data groups
  // groups_.clear();

  link next = links_[0];
  while (next) {
    // DG Data group
    data_groups_.emplace_back(handle_, next);
    next = data_groups_.back().get_next_group();
  }
}

} // namespace mdf
