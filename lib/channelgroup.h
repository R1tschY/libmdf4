/*
 * channelgroup.h
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

#ifndef CHANNELGROUP_H_
#define CHANNELGROUP_H_

#include "detail/mdf4.h"
#include "block.h"
#include "channel.h"

namespace mdf
{

class data_group;

// CG Channel Group
class channel_group : public block
{
public:
  channel_group(const data_group* dg, link l);

  uint64_t get_next_group() const { return links_[0]; }
  const data_group* get_data_group() const { return data_group_; }
  const std::vector<channel>& get_channels() const { return channels_; }

  // Channel Group
  uint64_t get_cycle_count() const { return cg_cycle_count_; }
  uint32_t get_data_bytes() const { return cg_data_bytes_; }
  uint32_t get_inval_bytes() const { return cg_inval_bytes_; }

private:
  std::vector<uint64_t> links_;
  std::vector<channel> channels_;

  uint64_t cg_cycle_count_;
  uint32_t cg_data_bytes_;
  uint32_t cg_inval_bytes_;

  const data_group* data_group_;

  void parse_channels();
};

} // namespace mdf

#endif // CHANNELGROUP_H_
