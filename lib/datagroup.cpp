/*
 * group.cpp
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

#include "datagroup.h"

#include "detail/mdf4.h"
#include "detail/rawfile.h"

#include <vector>

namespace mdf {

data_group::data_group(std::shared_ptr<rawfile>& file, uint64_t l) :
    block(file, l), links_(), channel_groups_(), rec_id_size_()
{
  file->seek(l);
  block_header header = prase_block_header(file.get(), make_id('D', 'G'));
  file->read_to_container(links_, header.link_count);

  file->read(rec_id_size_);
  if (rec_id_size_ != 0) {
    throw std::invalid_argument("unsorted MDF4 files not supported");
  }

  prase_channel_groups();
}

void data_group::prase_channel_groups() {
  link next = links_[1];
  while (next) {
    channel_groups_.emplace_back(this, next);
    next = channel_groups_.back().get_next_group();
  }
}

std::string data_group::get_metadata_comment() const {
  return prase_tx(file_.get(), links_[3]);
}



std::vector<std::string> data_group::get_recs(rawfile* file, std::vector<link> dt_links) const {
    std::vector<std::string> data;

    for (std::size_t i = 0; i < dt_links.size(); i++) {
        file->seek(dt_links[i]);
        block_header dt = prase_block_header(file, make_id('D', 'T'));
        uint64_t ndt_Bytes = dt.length - 24;
//        uint64_t ndt_Rec = ndt_Bytes / (get_data_bytes() + get_inval_bytes());
        data.push_back(std::string());
        file->read_to_container(data.back(), ndt_Bytes);
    }
    return data;
}

/*std::string _mdf4_get_recs_one(rawfile* file, data_group* dg, std::vector<link> dt_links, std::size_t i) {
  file->seek(dt_links[i]);
  block_header dt = prase_block_header(file, make_id('D', 'T'));
  uint64_t ndt_Bytes = dt.length - 24;

    std::string data;
    file->read_to_container(data[i], ndt_Bytes);
    if (ctx.ssRest.size() > 0) {
        data = ctx.ssRest + data;
        ndt_Bytes = data.size();
        ctx.ssRest.clear();
    }
    uint64_t nRest = ndt_Bytes % (dg->cg_data_bytes_ + dg->cg_inval_bytes_);
    if (nRest != 0) {
        ctx.ssRest.assign(data, ndt_Bytes-nRest+1, nRest);
        data.assign(data, 0, ndt_Bytes-nRest);
    }
    return data;
}*/



/*
Erstmal Vektor von DT-Links aufbauen, dann über alle DT-Blöcke gehen und die
Daten in 1000er-Blöcken nach oGroup.data[i] kopieren.
*/
void data_group::read_DL(rawfile* file, link pos) const {
//  int nExtra = 0;
//  int dt_first = 0;
  std::vector<link> dt_extra;

  link next = pos;
  while (next) {
    file->seek(next);
    block_header dl = prase_block_header(file, make_id('D', 'L'));

    std::vector<link> dl_links;
    file->read_to_container(dl_links, dl.link_count);

    dt_extra.insert(dt_extra.end(), dl_links.begin() + 1, dl_links.end());

    /*
                // Wird nicht benötigt
                dl_flags = fread_uint8(fp, 1);
                reserved = fread_uint8(fp, 3);
                dl_count = fread_uint32(fp, 1);
                if (dl_flags & 1) {
                    dl_equal_length = fread_uint64(fp, 1);
                } else {
                    dl_offset = fread_uint64(fp, dl_count);
                }*/


    next = dl_links[0];
  }

  data_.get() = get_recs(file, dt_extra);
}

void data_group::read_DT(rawfile* file, data_group*, link pos) const {
  data_.get() = get_recs(file, { pos });
}

const std::vector<std::string>& data_group::get_data() const {
  if (!data_) {
    data_ = std::move(std::vector<std::string>());
    read_DL(file_.get(), links_[2]);
  }

  return data_.get();
}

} // namespace mdf
