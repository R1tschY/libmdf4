/*
 * channelgroup.cpp
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

#include "channelgroup.h"

#include "datagroup.h"

namespace mdf
{

channel_group::channel_group(const data_group* dg, link l) :
    block(dg->get_file(), l), links_(), channels_(), data_group_(dg)
{
  file_->seek(l);
  block_header header = prase_block_header(file_.get(), make_id('C', 'G'));
  file_->read_to_container(links_, header.link_count);

  cgblock cg;
  file_->read(cg);

  cg_cycle_count_ = cg.cycle_count;
  cg_data_bytes_ = cg.data_bytes;
  cg_inval_bytes_ = cg.inval_bytes;

  parse_channels();
}

void channel_group::parse_channels() {
  link next = links_[1];
  while (next) {
    channels_.emplace_back(this, next);
    next = channels_.back().get_next_channel();
  }
}

} // namespace mdf
