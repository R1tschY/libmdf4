/*
 * channelconversation.cpp
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

#include "channelconversation.h"

namespace mdf {

channel_conversation::channel_conversation(const std::shared_ptr<rawfile>& file, uint64_t l) :
    file_(file), links_(), link_(l)
{
  file->seek(link_);

  // CC Channel conversion
  block_header header = prase_block_header(file_.get(), make_id('C', 'C'));
  file->read_to_container(links_, header.link_count);

  // p77 0: 1:1, 1: linear, ... 10: text-to-text
  file->read(block_);
  file->read_to_container(val_, block_.val_count);
}

} // namespace mdf
