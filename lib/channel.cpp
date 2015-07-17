/*
 * channel.cpp
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

#include "channel.h"

#include "datagroup.h"
#include "detail/memory.h"

namespace mdf {

channel::channel(const channel_group* cg, uint64_t l) :
    block(cg, l), links_(), cn_(), channel_group_(cg)
{
  file_->seek(l);

  // CN Channel
  block_header header = prase_block_header(file_.get(), make_id('C', 'N'));
  file_->read_to_container(links_, header.link_count);

  file_->read(cn_);

  if (links_[3] != 0) {
    // M4_SI
    source_information_ = source_information(this, links_[3]);
  }

  if (links_[4] != 0) {
    // M4_CC
    channel_conversation_ = channel_conversation(file_, links_[4]);
  }
}

std::string channel::get_name() const {
  return prase_tx(file_.get(), links_[2]);
}

std::string channel::get_metadata_unit() const {
  return prase_tx(file_.get(), links_[6]);
}

std::string channel::get_metadata_comment() const {
  return prase_tx(file_.get(), links_[7]);
}

channel::data_type channel::get_data_type() const {
  return static_cast<data_type>(cn_.data_type);
}

template<typename T>
T reverse_byte_order(T* value) {
  T result;

  char* ptr = reinterpret_cast<char*>(value) + sizeof(T) - 1;
  char* rptr = reinterpret_cast<char*>(&result);

  for (std::size_t i = 0; i < sizeof(T); i++) {
    *rptr = *ptr;

    ptr--;
    rptr++;
  }

  return result;
}

template<typename T>
static void convert_none(const channel_conversation&, T&) { }

template<typename T>
static void convert_linear(const channel_conversation& cc, T& value) {
  value = cc.val_[1] * value + cc.val_[0];
}

template<typename T>
static void convert_rational(const channel_conversation& cc, T& value) {
  auto o = cc.val_[0] * value * value + cc.val_[1] * value + cc.val_[2];
  auto u = cc.val_[3] * value * value + cc.val_[4] * value + cc.val_[5];
  value = o/u;
}

template<typename From, typename To>
static To convert_type(const void* void_ptr) {
  const From* ptr = static_cast<const From*>(void_ptr);
  return static_cast<To>(*ptr);
}

template<typename From, typename To>
static To convert_from_be_type(const void* void_ptr) {
  From from = *static_cast<const From*>(void_ptr);
  return static_cast<To>(reverse_byte_order(&from));
}

typedef double (*convert_type_func)(const void*);
typedef void (*convert_func)(const channel_conversation& cc, double& value);

static convert_func get_conv_func(const boost::optional<channel_conversation>& cc) {
  if (cc) {
    switch (cc->block_.type) {
    case 0: return convert_none<double>; break;
    case 1: return convert_linear<double>; break;
    case 2: return convert_rational<double>; break;
    default: throw error("Conversation type not supported");
    }
  } else {
    return convert_none<double>;
  }
}


void channel::get_data_real(std::vector<double>& data) const {
  convert_func conv_func = get_conv_func(channel_conversation_);

  if (get_type() == 3) {
    for (std::size_t i = 0; i < channel_group_->get_cycle_count(); i++) {
      data.push_back(static_cast<double>(i));

      // Channel Conversion
      if (channel_conversation_) {
        conv_func(channel_conversation_.get(), data.back());
      }
    }

    return;
  }

  convert_type_func conv_type_func;
  switch (cn_.data_type) {
  case 0: // unsigned int, little endian
    switch (cn_.bit_count) {
    case 8: conv_type_func = convert_type<uint8_t, double>; break;
    case 16: conv_type_func = convert_type<uint16_t, double>; break;
    case 32: conv_type_func = convert_type<uint32_t, double>; break;
    case 64: conv_type_func = convert_type<uint64_t, double>; break;
    default: throw error("Bit count of integer type not supported");
    }
    break;

 case 1: // unsigned int, big endian
    switch (cn_.bit_count) {
    case 8: conv_type_func = convert_from_be_type<uint8_t, double>; break;
    case 16: conv_type_func = convert_from_be_type<uint16_t, double>; break;
    case 32: conv_type_func = convert_from_be_type<uint32_t, double>; break;
    case 64: conv_type_func = convert_from_be_type<uint64_t, double>; break;
    default: throw error("Bit count of integer type not supported");
    }
    break;

  case 2: // int, little endian
    switch (cn_.bit_count) {
    case 8: conv_type_func = convert_type<int8_t, double>; break;
    case 16: conv_type_func = convert_type<int16_t, double>; break;
    case 32: conv_type_func = convert_type<int32_t, double>; break;
    case 64: conv_type_func = convert_type<int64_t, double>; break;
    default: throw error("Bit count of integer type not supported");
    }
    break;

  case 3: // int, big endian
    switch (cn_.bit_count) {
    case 8: conv_type_func = convert_from_be_type<int8_t, double>; break;
    case 16: conv_type_func = convert_from_be_type<int16_t, double>; break;
    case 32: conv_type_func = convert_from_be_type<int32_t, double>; break;
    case 64: conv_type_func = convert_from_be_type<int64_t, double>; break;
    default: throw error("Bit count of integer type not supported");
    }
    break;

  case 4: // real, little endian
    switch (cn_.bit_count) {
    case 32: conv_type_func = convert_type<float, double>; break;
    case 64: conv_type_func = convert_type<double, double>; break;
    default: throw error("Bit count of float type not supported");
    }
    break;

  case 5: // real, big endian
    switch (cn_.bit_count) {
    case 32: conv_type_func = convert_from_be_type<float, double>; break;
    case 64: conv_type_func = convert_from_be_type<double, double>; break;
    default: throw error("Bit count of float type not supported");
    }
    break;

/* else if (dt < 10) {
    // @bug String data types
    throw error("String data types not supported");
    return 1e10;
  } else if ( dt <= 14) {
    // @bug Complex data types
    //throw error("complex data types not supported");
    return 1e10;
  } else {
    //throw error("error in data file");
    return 1e10;
  }*/

  default:
    throw error("wrong data type requested");
  }

  if (cn_.bit_offset != 0) {
    throw error("Bit offset in channel data not supported");
  }
  
  auto n = channel_group_->get_cycle_count();
  auto nRecLen = channel_group_->get_data_bytes() + channel_group_->get_inval_bytes();

  const std::vector<std::string>& complete_data = channel_group_->get_data_group()->get_data();
  for (std::size_t i = 0; i < complete_data.size(); i++) {
    auto nBytes = complete_data[i].size();
    auto nRec = std::min<uint64_t>(n, nBytes / nRecLen);

/*    const char* ptr = complete_data[i].data() + cn_.byte_offset;
    for (std::size_t i = 0; i < nRec; i++) {
      data.push_back(conv_type_func(ptr));
      ptr += nRecLen;
    }*/

    auto range = make_memory_range<char>(complete_data[i].data(), nRec, cn_.byte_offset, nRecLen - 1);
    for (const char& c : range) {
      data.push_back(conv_type_func(&c));

      // Channel Conversion
      if (channel_conversation_) {
        conv_func(channel_conversation_.get(), data.back());
      }
    }
  }
}

} // namespace mdf
