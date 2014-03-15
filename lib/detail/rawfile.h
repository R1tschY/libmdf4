/*
 * systemfile.h
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

#ifndef LIBMDF_RAWFILE_H_
#define LIBMDF_RAWFILE_H_

#include <cstdio>
#include <memory>
#include <system_error>

#include <boost/utility/string_ref.hpp>

#include "macros.h"

namespace mdf {

class io_error : public std::system_error {
public:
  io_error(int errnum) :
    system_error(errnum, std::generic_category())
  { }

  io_error() :
    system_error(errno, std::generic_category())
  { }

  virtual ~io_error() noexcept;
};


class rawfile {
  NOT_COPYABLE(rawfile);

public:
  enum class seek_orgin {
    begin = SEEK_SET, // Beginning of file
    current = SEEK_CUR, // Current position of the file pointer
    end = SEEK_END  // End of file
  };

  constexpr rawfile() noexcept :
    file_handle_()
  { }

  rawfile(FILE* file) noexcept :
    file_handle_(file)
  { }

  rawfile(boost::string_ref filename, boost::string_ref mode) :
    file_handle_()
  {
    open(filename, mode);
  }

  void open(boost::string_ref filename, boost::string_ref mode);

  void warp(FILE* file) noexcept
  {
    file_handle_.reset(file);
  }

  FILE* release() noexcept {
    return file_handle_.release();
  }

  bool is_open() const noexcept {
    return file_handle_ != nullptr;
  }

  explicit operator bool() const noexcept {
    return is_open() && good();
  }

  template<typename T>
  void read(T& t) {
    if (fread(&t, sizeof(T), 1, file_handle_.get()) != 1)
      throw io_error();
  }
  template<typename T>
  void read(T* t, std::size_t n) {
    if (fread(t, sizeof(T), n, file_handle_.get()) != n)
      throw io_error();
  }
  void read(char& t);

  template<typename T, std::size_t N>
  std::size_t read_same(T (&t)[N]) noexcept {
    return fread(&t, sizeof(T), N, file_handle_.get());
  }

  template<typename T>
  void read_partial(T& t, std::size_t n) {
    if (fread(&t, sizeof(char), n, file_handle_.get()) != n)
      throw io_error();
  }
  template<typename T>
  void read_partial(T& t, std::size_t pos, std::size_t n) {
    if (fread(&t + pos, sizeof(char), n, file_handle_.get()) != n)
      throw io_error();
  }

  template<typename T>
  void read_to_container(T& t, std::size_t n) {
    t.resize(n);
    if (fread(
        const_cast<typename T::pointer>(t.data()),
        sizeof(typename T::value_type),
        n,
        file_handle_.get()) != n)
      throw io_error();
  }

  template<typename T>
  void write(const T& t) {
    if (fwrite(&t, sizeof(T), 1, file_handle_.get()) != 1)
      throw io_error();
  }
  template<typename T>
  void write(T* t, std::size_t n) {
    if (fwrite(t, sizeof(T), n, file_handle_.get()) != n)
      throw io_error();
  }
  void write(char t);

  template<typename T, std::size_t N>
  std::size_t write_same(const T (&t)[N]) noexcept {
    return fwrite(&t, sizeof(T), N, file_handle_.get());
  }

  template<typename T>
  void write_from_container(const T& t) {
    if (fwrite(t.data(), sizeof(T::value_type), t.size(), file_handle_.get()) != t.size())
      throw io_error();
  }

  void write_text(boost::string_ref text) {
    write(text.data(), text.size());
  }

  void write_formated(const char* format, ...);

  void seek(long int offset, seek_orgin origin = seek_orgin::begin);

  std::size_t tell() noexcept {
    return ftell(file_handle_.get());
  }

  void close() noexcept {
    file_handle_.reset(nullptr);
  }

  bool eof() const noexcept {
    return feof(file_handle_.get());
  }

  bool fail() const noexcept {
    return ferror(file_handle_.get());
  }

  bool good() const noexcept {
    return !eof() && !fail();
  }

private:
  class file_closer
  {
  public:
    constexpr file_closer() noexcept = default;

    void
    operator()(FILE* file) const
    {
      fclose(file);
    }
  };
  typedef std::unique_ptr<FILE, file_closer> file_t;

  file_t file_handle_;
};

} // namespace mdf

#endif // LIBMDF_RAWFILE_H_
