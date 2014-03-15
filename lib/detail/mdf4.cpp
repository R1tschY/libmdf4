/*
 * mdf4.cpp
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

#include "mdf4.h"

#include <algorithm>
#include <cstring>

#include "rawfile.h"
#include "xml.h"

namespace mdf {

const char* error::what() const throw () {
  return msg_;
}

block_header prase_block_header(rawfile* file, uint16_t id) {
  block_header header;
  file->read(header);

  if (header.double_hash != make_id('#', '#')) {
    // not a block (or corrupted)
    throw error("format error: to read block is not a block!");
  }

  if (header.id != id) {
    // not the block searching for
    std::string o(reinterpret_cast<char*>(&header.id), 2);
    std::string t(reinterpret_cast<char*>(&id), 2);
    printf("%s != %s\n", o.c_str(), t.c_str());
    throw error("format error: given block id is not matching readed blocks id!");
  }

  return header;
}

block_header prase_block_header(rawfile* file) {
  block_header header;
  file->read(header);

  if (header.double_hash != make_id('#', '#')) {
    // not a block (or corrupted)
    throw error("format error: to read block is not a block!");
  }

  return header;
}

// M4_TX Text utf8 or Metadata block (XML)
std::string prase_tx(rawfile* file, link l) {
  std::string result;

  if (l == 0) return result;

  file->seek(l, rawfile::seek_orgin::begin);

  block_header header = prase_block_header(file);
  if (header.id != make_id('T', 'X') && header.id != make_id('M', 'D')) {
    throw error("format error: not a MD or TX block!");
  }
  if (header.length <= sizeof(block_header)) {
    return result;
  }

  file->read_to_container(result, header.length - sizeof(block_header));
  result.resize(strlen(result.c_str()));
  return result;
}

std::string extract_tx_from_xml(std::string xml) {
  const char tx_begin[] = "<TX>";
  const char tx_end[] = "</TX>";
  std::string result;

  auto begin = std::search(RANGE(xml), tx_begin, tx_begin + 4);
  if (begin == xml.end()) {
    return "<TX>";
  }

  auto end = std::search(begin, xml.end(), tx_end, tx_end + 5);
  if (end == xml.end()) {
    return "</TX>";
  }

  result.assign(begin + 4, end);

  std::string Result;
  decode_entities(result, Result);

  return Result;
}

} // namespace mdf
