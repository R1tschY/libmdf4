/*
 * channel.h
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

#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <vector>
#include <memory>
#include <boost/optional.hpp>

#include "block.h"
#include "sourceinformation.h"
#include "channelconversation.h"
#include "detail/mdf4.h"
#include "detail/macros.h"

namespace mdf {

class channel_group;

class channel : block {
public:
  enum class data_type {
    // integer types, little endian
    int8,
    uint8,
    int32,
    uint32,
    int64,
    uint64,

    // float types
    real32,
    real64,

    // other types
    string,
    complex
  };

  channel(const channel_group* cg, uint64_t l);

  const boost::optional<source_information>& get_source_information() const
      { return source_information_; }

  const boost::optional<channel_conversation>& get_channel_conversation() const
      { return channel_conversation_; }

  std::string get_name() const;
  std::string get_metadata_unit() const;
  std::string get_metadata_comment() const;

  void get_data_real(std::vector<double>& data) const;

  data_type get_data_type() const;
  unsigned get_type() const { return cn_.type; }
  unsigned get_sync_type() const { return cn_.sync_type; }

  uint64_t get_next_channel() const { return links_[0]; }

private:
  std::vector<uint64_t> links_;

  boost::optional<source_information> source_information_;
  boost::optional<channel_conversation> channel_conversation_;

  cnblock cn_;

  const channel_group* channel_group_;

  void get_rawdata(void* data);
};

} // namespace mdf

#endif // CHANNEL_H_
