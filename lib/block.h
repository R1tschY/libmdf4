/*
 * block.h
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

#ifndef BLOCK_H_
#define BLOCK_H_

#include <memory>

#include "detail/mdf4.h"

namespace mdf {

class rawfile;

class block {
public:
  block(const std::shared_ptr<rawfile>& file, link l);
  block(const block* parent, link l);

  virtual ~block();

  const std::shared_ptr<rawfile>& get_file() const { return file_; };

protected:
  mutable std::shared_ptr<rawfile> file_;
  const block* parent_;
};

} // namespace mdf

#endif // BLOCK_H_
