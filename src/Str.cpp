//
// Created by bodguy on 2020/01/01.
//

#include "Str.h"
#include <cstring>

const Str::size_type Str::initial_capacity = 32;

std::ostream& operator <<(std::ostream& os, const Str& str) {
  os << str.to_utf8_str();
  return os;
}

Str operator +(const Str& lhs, const Str& rhs) {
  Str out(lhs);
  out += rhs;

  return out;
}

Str::Str() : buffer(nullptr), reserved_size(0L), used_size(0L) {
  grow_buffer(initial_capacity, false);
}

Str::Str(const pointer str) : buffer(nullptr), reserved_size(0L), used_size(0L) {
  copy_string_to_buffer(str, ::length(str));
}

Str::Str(const i8* str) : buffer(nullptr), reserved_size(0L), used_size(0L) {

}

Str::Str(const value_type c) : buffer(nullptr), reserved_size(0L), used_size(0L) {
  grow_buffer(initial_capacity, false);
  buffer[0] = c;
  buffer[1] = '\0';
  used_size = 1;
}

Str::Str(const Str& other) : buffer(nullptr), reserved_size(0L), used_size(0L) {
  copy_string_to_buffer(other.buffer, other.used_size);
}

Str::~Str() {
  free(buffer);
}

Str::size_type Str::size() const {
  return used_size;
}

Str::size_type Str::length() const {
  return used_size;
}

Str::size_type Str::capacity() const {
  return reserved_size;
}

void Str::reserve(size_type new_size) {
  grow_buffer(new_size, true);
}

Str::pointer Str::c_str() const {
  return buffer;
}

Str& Str::operator =(const Str& other) {
  if (this != &other) {
    copy_string_to_buffer(other.buffer, other.used_size);
  }

  return *this;
}

Str& Str::operator +=(const Str& other) {
  size_type total_size = used_size + other.used_size;
  if (total_size > reserved_size) {
    reserve(recommend_reserve_size(total_size));
  }
//  strcat(buffer, other.buffer);
  used_size = total_size;
  buffer[used_size] = '\0';

  return *this;
}

bool Str::operator ==(const Str& other) const {
  return 0 == ::compare(buffer, other.buffer, ::length(buffer), ::length(other.buffer));
}

Str::reference Str::operator [](size_type index) {
  return buffer[index];
}

Str::reference Str::operator [](size_type index) const {
  return buffer[index];
}

Str::reference Str::at(size_type index) {
  assert(index < used_size);
  return buffer[index];
}

Str::reference Str::at(size_type index) const {
  assert(index < used_size);
  return buffer[index];
}

bool Str::empty() const {
  return used_size == 0;
}

Str::size_type Str::codepoint_count(usize begin, usize end) const {

}

std::string Str::to_utf8_str() const {
  Str::const_iterator start = cbegin();
  Str::const_iterator end = cend();
  std::string result;

  while (start != end) {
    u32 cp = mask16(*start++);

    if (is_lead_surrogate(cp)) {
      u32 trail_surrogate = mask16(*start++);
      cp = (cp << 10) + trail_surrogate + SUR_OFFSET;
    }
    result.append(to_utf8_char(cp));
  }

  return result;
}

std::string Str::to_utf8_char(u32 cp) const {
  std::string result;

  if (cp < 0x80) {
    // one octets
    result.push_back(static_cast<u8>(cp));
  } else if (cp < 0x800) {
    // two octets
    result.push_back(static_cast<u8>((cp >> 6) | 0xC0));
    result.push_back(static_cast<u8>((cp & 0x3F) | 0x80));
  } else if (cp < 0x10000) {
    // three octets
    result.push_back(static_cast<u8>((cp >> 12) | 0xE0));
    result.push_back(static_cast<u8>(((cp >> 6) & 0x3F) | 0x80));
    result.push_back(static_cast<u8>((cp & 0x3F) | 0x80));
  } else {
    // four octets
    result.push_back(static_cast<u8>((cp >> 18) | 0xF0));
    result.push_back(static_cast<u8>(((cp >> 12) & 0x3F) | 0x80));
    result.push_back(static_cast<u8>(((cp >> 6) & 0x3F) | 0x80));
    result.push_back(static_cast<u8>((cp & 0x3F) | 0x80));
  }

  return result;
}

Str::iterator Str::begin() {
  return { buffer, 0 };
}

Str::iterator Str::end() {
  return { buffer, used_size };
}

Str::const_iterator Str::cbegin() const {
  return { buffer, 0 };
}

Str::const_iterator Str::cend() const {
  return { buffer, used_size };
}

Str::size_type Str::recommend_reserve_size(size_type str_len) {
  return (str_len + 1) * 2;
}

void Str::grow_buffer(size_type new_size, bool copy_data) {
  if (new_size > reserved_size) {
    pointer new_buffer = (pointer)malloc(new_size * sizeof(value_type));

    if (used_size && copy_data) {
      // copy the old to new buffer
      memcpy(new_buffer, buffer, used_size);
      new_buffer[used_size] = '\0';
    }

    free(buffer);
    buffer = new_buffer;
    reserved_size = new_size;
  }
}

void Str::copy_string_to_buffer(const pointer str, size_type str_len) {
  if (str_len >= reserved_size) {
    grow_buffer(recommend_reserve_size(str_len), false);
  }
  used_size = str_len;
  memcpy(buffer, str, str_len);
  buffer[used_size] = '\0';
}

// generates a UTF-8 encoding from a 32 bit UCS-4 character.
void Str::get_utf8_encoding(u32 in, utf8_encoding& out, int& out_size, bool default_order) {
  if (!default_order) {
    // reverse byte order what ever ordering is applied (little or big)
    in = ((in & 0x000000ff) << 24) + ((in & 0x0000ff00) << 8) + ((in & 0x00ff0000) >> 8) + ((in & 0xff000000) >> 24);
  }

  if (in < 0x80) { // 1000 0000
    // 1 byte encoding
    out[0] = (char)in;
    out_size = 1;
  } else if (in < 0x800) { // 1000 0000 0000
    // 2 byte encoding
    // 1100 0000 (0xC0)
    out[0] = 0xC0 + ((in & 0x7C0) >> 6);
    out[1] = 0x80 + (in & 0x3F);
    out_size = 2;
  } else if (in < 0x10000) { // 0001 0000 0000 0000 0000
    // 3 byte encoding
    // 1110 0000 (0xE0)
    out[0] = 0xE0 + ((in & 0xF000) >> 12);
    out[1] = 0x80 + ((in & 0xFC0) >> 6);
    out[2] = 0x80 + (in & 0x3F);
    out_size = 3;
  } else {
    // 4 byte encoding
    // 1111 0000 (0xF0)
    out[0] = 0xF8 + ((in & 0x1C0000) >> 18);
    out[1] = 0x80 + ((in & 0x3F000) >> 12);
    out[2] = 0x80 + ((in & 0xFC0) >> 6);
    out[3] = 0x80 + (in & 0x3F);
    out_size = 4;
  }
}

void Str::get_utf8_encoding(u16 in, utf8_encoding& out, int& out_size, bool default_order) {
  if (!default_order) {
    in = ((in & 0x00ff) << 8) + ((in & 0xff00) >> 8);
  }

  get_utf8_encoding((u32)in, out, out_size, true);
}

u16 Str::from_utf8(const u8* v) const {

}

u16 Str::from_utf8_lossy(const u8* v) const {

}

//u32 Str::from_utf8(const u8* utf8_data) {
//  if (utf8_data[0] < 0x80) {
//    // 1 byte
//    return (u32)utf8_data[0];
//  } else if (utf8_data[0] < 0xE0) {
//    // 2 byte
//    return ((utf8_data[0] & 0x1F) << 6) + (utf8_data[1] & 0x3F);
//  } else if (utf8_data[0] < 0xF0) {
//    // 3 byte
//    return ((utf8_data[0] & 0xF) << 12) + ((utf8_data[1] & 0x3F) << 6) + (utf8_data[2] & 0x3F);
//  } else {
//    // 4 byte
//    return ((utf8_data[0] & 0x7) << 18) + ((utf8_data[1] & 0x3F) << 12) + ((utf8_data[2] & 0x3F) << 6) + (utf8_data[3] & 0x3F);
//  }
//}