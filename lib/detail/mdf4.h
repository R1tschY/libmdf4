/*
 * mdf4.h
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

#ifndef MDF_H_
#define MDF_H_

#include <cstdint>
#include <exception>

#include "macros.h"
#include "rawfile.h"

namespace mdf {

// mdf types

typedef uint16_t boolean; // BOOL type
typedef double real;
typedef uint64_t link;

struct idblock {
  char file_id[8];
  char format_id[8];
  char program_id[8];
  uint16_t default_byte_order; // for 3.x
  uint16_t default_floating_point_format; // for 3.x
  uint16_t version_number;
  uint16_t code_page_number; // for 3.x
} PACKED;

struct block_header {
  uint16_t double_hash;
  uint16_t id;
  uint32_t _unkown;
  uint64_t length;
  uint64_t link_count;
} PACKED;

/// HD Header Block
struct hdblock {
  /// Absolute start time in nanoseconds since midnight Jan 1st, 1970
  uint64_t start_time_ns;

  /// Time zone offset in minutes
  uint16_t tz_offset_min;

  /// Daylight saving time (DST) offset in minutes
  uint16_t dst_offset_min;

  /// Timeflags
  /// 0b10 -> OFFSET_VALID
  uint8_t time_flags;

  /// Time quality class
  /// 0 = TIME_SRC_PC
  uint8_t time_class;

  /// Flags
  uint8_t flags;

  /// Reserved
  int8_t _reserved;

  /// Start angle in radians at start of measurement
  real start_angle_rad;

  /// Start distance in meters at start of measurement
  real start_distance_m;
};

// CG Channel Group
struct cgblock {
  uint64_t _unknown1;
  uint64_t cycle_count;
  uint16_t flags;
  uint8_t _unknown2[6];
  uint32_t data_bytes;
  uint32_t inval_bytes;
} PACKED;

// CN Channel
struct cnblock {
  uint8_t type;
  uint8_t sync_type;
  uint8_t data_type;
  uint8_t bit_offset;
  uint32_t byte_offset;
  uint32_t bit_count;
  uint32_t flags;
  uint32_t inval_bit_pos;
  uint8_t precision;
  uint8_t _unknown[3];
  double val_range[2];
  double limit[4];
} PACKED;

// SI Source Information
struct siblock {
  uint8_t _unknown[3];
} PACKED;

// CC Channel conversion
struct ccblock {
  uint8_t type;
  uint8_t precision;
  uint16_t flags;
  uint16_t ref_count;
  uint16_t val_count;
  double phy_range[2];
} PACKED;

// intern types

constexpr uint16_t make_id(char c1, char c2) {
  return static_cast<uint16_t>((static_cast<uint16_t>(c2) << 8u) + static_cast<uint16_t>(c1));
}

class error : public std::exception {
public:
  error(const char* msg) throw() : msg_(msg) { }

  virtual const char* what() const throw();

private:
  const char* msg_;
};

// prase blocks

block_header prase_block_header(rawfile* file, uint16_t id);
block_header prase_block_header(rawfile* file);
std::string prase_tx(rawfile* file, link l);

std::string extract_tx_from_xml(std::string);

} // namespace mdf

#endif // MDF_H_
