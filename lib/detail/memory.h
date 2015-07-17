/*
 * memory.h
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

#ifndef MEMORY_H_
#define MEMORY_H_

#include <boost/range/iterator_range_core.hpp>

namespace mdf {

template<typename T, typename T_nonconst = T>
class memory_iterator : public std::iterator<std::random_access_iterator_tag, T> {
public:
  typedef memory_iterator self_type;

  // ctor
  constexpr
  memory_iterator(T* pos, std::size_t byte_offset) :
    pos_(pos), offset_(sizeof(T) + byte_offset)
  { }

  // convert from iterator to const_iterator
  constexpr
  memory_iterator(const memory_iterator<T_nonconst> &other) :
    pos_(other.pos_), offset_(other.offset_)
  { }

  friend class memory_iterator<const T>;

  T& operator*() {
    return *pos_;
  }
  
  self_type& operator+=(int n) {
    typedef const char* char_pointer;
    char_pointer& ptr = static_cast<char_pointer&>(pos_);
    ptr += offset_ * n;
    return *this;
  }

  self_type& operator+=(std::size_t n) {
    typedef const char* char_pointer;
    char_pointer& ptr = static_cast<char_pointer&>(pos_);
    ptr += offset_ * n;
    return *this;
  }
  
  self_type& operator-=(std::size_t n) {
    typedef const char* char_pointer;
    char_pointer& ptr = static_cast<char_pointer&>(pos_);
    ptr -= offset_ * n;
    return *this;
  }

  // functions below are using only functions above
  T* operator->()  {
    return &(operator*());
  }

  self_type& operator-=(int n) {
    return *this += -n;
  }

  constexpr
  self_type operator+(std::size_t n) const {
    return self_type(*this) += n;
  }

  constexpr
  self_type operator-(std::size_t n) const {
    return self_type(*this) -= n;
  }

  self_type& operator++() {
    return (*this += 1);
  }

  const self_type operator++(int) {
    const self_type old = *this;
    ++(*this);
    return old;
  }

  self_type& operator--() {
    return (*this -= 1);
  }

  const self_type operator--(int) {
    const self_type old = *this;
    --(*this);
    return old;
  }

  constexpr
  bool operator==(const self_type& other) const {
    return pos_ == other.pos_;
  }

  constexpr
  bool operator!=(const self_type& other) const {
    return pos_ != other.pos_;
  }

private:
  T* pos_;
  std::size_t offset_;
};

template<typename T>
boost::iterator_range<memory_iterator<T> > make_memory_range(char* memory,
    std::size_t count, std::size_t start_byte_offset = 0,
    std::size_t byte_offset = 0)
{
  memory_iterator<T> begin(static_cast<T*>(memory + start_byte_offset),
      byte_offset);
  return boost::make_iterator_range(begin, begin + count);
}

template<typename T>
boost::iterator_range<memory_iterator<const T> > make_memory_range(const char* memory,
    std::size_t count, std::size_t start_byte_offset = 0,
    std::size_t byte_offset = 0)
{
  memory_iterator<const T> begin(static_cast<const T*>(memory + start_byte_offset),
      byte_offset);
  return boost::make_iterator_range(begin, begin + count);
}

} // namespace mdf

#endif // MEMORY_H_
