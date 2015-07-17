/*
 * systemfile.cpp
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

#include "rawfile.h"

#include <stdarg.h>

namespace mdf {

io_error::~io_error() noexcept
{ }

void rawfile::open(boost::string_ref filename, boost::string_ref mode)  {
  if (*filename.end() != '\0') {
    // filename ist not null terminated
    std::string tmp(filename.begin(), filename.end());
    file_handle_.reset(fopen(tmp.c_str(), mode.data()));

  } else {
    file_handle_.reset(fopen(filename.data(), mode.data()));
  }
  if (!file_handle_) throw io_error();
}

void rawfile::read(char& t) {
  int c = fgetc(file_handle_.get());
  if (c == EOF)
    throw io_error();
    
  t = static_cast<char>(c);
}

void rawfile::write(char t) {
  if (fputc(t, file_handle_.get()) != t)
    throw io_error();
}

void rawfile::write_formated(const char* format, ...) {
  va_list vl;
  va_start(vl, format);
  int chars = vfprintf(file_handle_.get(), format, vl);
  va_end(vl);
  if (chars < 0) {
    throw io_error();
  }
}

void rawfile::seek(long int offset, seek_orgin origin) {
  if (fseek(file_handle_.get(), offset, static_cast<int>(origin)) != 0)
    throw io_error();
}

} // namespace mdf
