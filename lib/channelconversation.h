/*
 * channelconversation.h
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

#ifndef CHANNELCONVERSATION_H_
#define CHANNELCONVERSATION_H_

#include <vector>
#include <memory>

#include "detail/mdf4.h"

namespace mdf {

class channel_conversation {
public:
  channel_conversation();
  channel_conversation(const std::shared_ptr<rawfile>& file, uint64_t l);

public:
  mutable std::shared_ptr<rawfile> file_;
  std::vector<uint64_t> links_;

  uint64_t link_; // link to this channel in the file

  ccblock block_;
  std::vector<double> val_;
};

} // namespace mdf

#endif // CHANNELCONVERSATION_H_
