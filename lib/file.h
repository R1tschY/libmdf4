/*
 * file.h
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

#ifndef LIBMDF_FILE_H_
#define LIBMDF_FILE_H_

#include <cstdio>
#include <vector>
#include <memory>

#include <boost/optional.hpp>

#include "detail/rawfile.h"
#include "datagroup.h"

namespace mdf {

class file {
public:
  file() :
    handle_(), file_version_(),
    links_(), data_groups_()
  { }

  file(const std::string& filename) :
    handle_(), file_version_(),
    links_(), data_groups_()
  {
    open(filename);
  }

  file(const char* filename) :
    handle_(), file_version_(),
    links_(), data_groups_()
  {
    open(filename);
  }

  void open(const std::string& filename) {
    open(filename.c_str());
  }

  void open(const char* filename);

  explicit operator bool() const {
    return good();
  }

  bool good() const {
    return handle_ && handle_->is_open() && handle_->good();
  }

  std::string get_metadata_comment() const;
  std::string get_comment() const;

  uint16_t get_mdf_version() const { return file_version_; }
  std::string get_mdf_version_string() const;

  std::string get_generator_name() const;

  const std::vector<data_group>& get_data_groups() const { return data_groups_; }

private:
  // file itself
  mutable std::shared_ptr<rawfile> handle_;

  // information about file
  uint16_t file_version_;

  //
  std::vector<uint64_t> links_;
  std::vector<data_group> data_groups_;

  void prase_idblock();
  void prase_basic_hdblock();
  void prase_groups();
};

} // namespace mdf

#endif // LIBMDF_FILE_H_
