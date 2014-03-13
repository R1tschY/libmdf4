/*
 * sourceinformation.cpp
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

#include "sourceinformation.h"
#include "detail/mdf4.h"

namespace mdf {

source_information::source_information(const block* parent, uint64_t l)  :
  block(parent, l), links_()
{
  file_->seek(l);

  // CN Channel
  block_header header = prase_block_header(file_.get(), make_id('S', 'I'));
  file_->read_to_container(links_, header.link_count);
}

std::string source_information::get_name() const {
  return prase_tx(file_.get(), links_[0]);
}

std::string source_information::get_path() const {
  return prase_tx(file_.get(), links_[1]);
}

std::string source_information::get_metadata_comment() const {
  return prase_tx(file_.get(), links_[2]);
}

} // namespace mdf

