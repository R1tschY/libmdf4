/*
 * group.h
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

#ifndef GROUP_H_
#define GROUP_H_

#include <memory>
#include <vector>

#include <boost/optional.hpp>

#include "block.h"
#include "channelgroup.h"

namespace mdf {

class rawfile;

class data_group : public block {
public:
  data_group(std::shared_ptr<rawfile>& file, uint64_t l);

  std::string get_metadata_comment() const;

  uint64_t get_next_group() const { return links_[0]; }

  const std::vector<channel_group>& get_channel_groups() const { return channel_groups_; }

  const std::vector<std::string>& get_data() const;

private:
  std::vector<uint64_t> links_;
  std::vector<channel_group> channel_groups_;

  uint8_t rec_id_size_;

  mutable boost::optional<std::vector<std::string> > data_;

  std::vector<std::string> get_recs(rawfile* file, std::vector<link> dt_links) const;
  void read_DL(rawfile* file, link pos) const;
  void read_DT(rawfile* file, data_group* dg, link pos) const;

  void prase_channel_groups();
};

} // namespace mdf

#endif // GROUP_H_
