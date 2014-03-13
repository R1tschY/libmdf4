/*
 * sourceinformation.h
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

#ifndef SOURCEINFORMATION_H_
#define SOURCEINFORMATION_H_

#include <vector>
#include <memory>

#include "block.h"

namespace mdf {

class source_information : block {
public:
  source_information(const block* parent, uint64_t l);

  std::string get_name() const;
  std::string get_path() const;
  std::string get_metadata_comment() const;

private:
  std::vector<uint64_t> links_;
};

} // namespace mdf

#endif // SOURCEINFORMATION_H_
