//
// Created by bodguy on 2020/01/01.
//

#ifndef UTF_FSTRING_H
#define UTF_FSTRING_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ostream>

#if defined(_MSC_VER)
#define FORCE_INLINE	__forceinline
#else	// defined(_MSC_VER)
#define	FORCE_INLINE inline __attribute__((always_inline))
#endif

class FString {
private:
  template <typename T>
  class Iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
    typedef size_t size_type;
    typedef typename std::iterator<std::bidirectional_iterator_tag, T>::pointer pointer;
    typedef typename std::iterator<std::bidirectional_iterator_tag, T>::value_type value_type;
    typedef typename std::iterator<std::bidirectional_iterator_tag, T>::difference_type difference_type;
    typedef typename std::iterator<std::bidirectional_iterator_tag, T>::reference reference;
  public:
    Iterator(pointer ptr, size_type start) { buf_ = &ptr[start]; }
    Iterator(pointer ptr) : buf_(ptr) {}
    FORCE_INLINE reference operator*() const { return *buf_; }
    FORCE_INLINE Iterator& operator++() { inc(); return *this; }
    FORCE_INLINE Iterator operator++(int) {
      Iterator copy(*this);
      inc();
      return copy;
    }
    FORCE_INLINE Iterator& operator--() { dec(); return *this; }
    FORCE_INLINE Iterator operator--(int) {
      Iterator copy(*this);
      dec();
      return copy;
    }
    FORCE_INLINE bool operator == (const Iterator& other) const { return buf_ == other.buf_; }
    FORCE_INLINE bool operator != (const Iterator& other) const { return buf_ != other.buf_; }
    FORCE_INLINE bool operator < (const Iterator& other) const { return buf_ < other.buf_; }
    FORCE_INLINE bool operator > (const Iterator& other) const { return buf_ > other.buf_; }
    FORCE_INLINE bool operator <= (const Iterator& other) const { return buf_ <= other.buf_; }
    FORCE_INLINE bool operator >= (const Iterator& other) const { return buf_ >= other.buf_; }

  private:
    FORCE_INLINE void inc() { ++buf_; }
    FORCE_INLINE void dec() { --buf_; }

    pointer buf_;
  };

public:
  typedef char value_type;
  typedef size_t size_type;
  typedef value_type* pointer;
  typedef value_type& reference;
  typedef Iterator<value_type> iterator;
  typedef Iterator<const value_type> const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  FString() : buffer(NULL), reserved_size(0L), used_size(0L) {
    grow_buffer(initial_capacity, false);
  }
  FString(const pointer str) : buffer(NULL), reserved_size(0L), used_size(0L) {
    copy_string_to_buffer(str, strlen(str));
  }
  FString(const value_type c) : buffer(NULL), reserved_size(0L), used_size(0L) {
    grow_buffer(initial_capacity, false);
    buffer[0] = c;
    buffer[1] = '\0';
    used_size = 1;
  }
  FString(const FString& other) : buffer(NULL), reserved_size(0L), used_size(0L) {
    copy_string_to_buffer(other.buffer, other.used_size);
  }
  ~FString() { free(buffer); }

  FORCE_INLINE size_type size() const { return used_size; }
  FORCE_INLINE size_type length() const { return used_size; }
  FORCE_INLINE size_type capacity() const { return reserved_size; }
  FORCE_INLINE void reserve(size_type new_size) { grow_buffer(new_size, true); }
  FORCE_INLINE const pointer c_str() const { return buffer; }

  FORCE_INLINE FString& operator=(const FString& other) {
    if (this != &other) {
      copy_string_to_buffer(other.buffer, other.used_size);
    }

    return *this;
  }

  FORCE_INLINE FString& operator+=(const FString& other) {
    size_type total_size = used_size + other.used_size;
    if (total_size > reserved_size) {
      reserve(recommend_reserve_size(total_size));
    }
    strcat(buffer, other.buffer);
    used_size = total_size;
    buffer[used_size] = '\0';

    return *this;
  }

  FORCE_INLINE bool operator==(const FString& other) const {
    return 0 == strcmp(buffer, other.buffer);
  }

  FORCE_INLINE reference operator[](size_type index) { return buffer[index]; }
  FORCE_INLINE reference operator[](size_type index) const { return buffer[index]; }
  FORCE_INLINE reference at(size_type index) { assert(index < used_size); return buffer[index]; }
  FORCE_INLINE reference at(size_type index) const { assert(index < used_size); return buffer[index]; }
  FORCE_INLINE bool empty() const { return used_size == 0; }

  friend std::ostream& operator<<(std::ostream& os, const FString& str) {
    os << str.c_str();
    return os;
  }

  friend FString operator+(const FString& lhs, const FString& rhs) {
    FString out(lhs);
    out += rhs;

    return out;
  }

private:
  static size_type recommend_reserve_size(size_type str_len) { return (str_len + 1) * 2; }
  static const size_type initial_capacity = 32;

  FORCE_INLINE void grow_buffer(size_type new_size, bool copy_data) {
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

  FORCE_INLINE void copy_string_to_buffer(const pointer str, size_type str_len) {
    if (str_len >= reserved_size) {
      grow_buffer(recommend_reserve_size(str_len), false);
    }
    used_size = str_len;
    memcpy(buffer, str, str_len);
    buffer[used_size] = '\0';
  }

  typedef unsigned char utf8_encoding[4];
  typedef unsigned char uchar8;
  typedef unsigned short uchar16;
  typedef unsigned int uchar32;

  // generates a UTF-8 encoding from a 32 bit UCS-4 character.
  FORCE_INLINE void get_utf8_encoding(uchar32 in, utf8_encoding& out, int& out_size, bool default_order = true) {
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
  // generates a UTF-8 encoding from a 16 bit UCS-2 character.
  FORCE_INLINE void get_utf8_encoding(uchar16 in, utf8_encoding& out, int& out_size, bool default_order = true) {
    if (!default_order) {
      in = ((in & 0x00ff) << 8) + ((in & 0xff00) >> 8);
    }

    get_utf8_encoding((uchar32)in, out, out_size, true);
  }
  //  Converts a UTF8 encoded character into a 32 bit single code
  FORCE_INLINE uchar32 utf8_char_to_unicode(const uchar8* utf8_data) {
    if (utf8_data[0] < 0x80) {
      // 1 byte
      return (uchar32)utf8_data[0];
    } else if (utf8_data[0] < 0xE0) {
      // 2 byte
      return ((utf8_data[0] & 0x1F) << 6) + (utf8_data[1] & 0x3F); // 00111111
    } else if (utf8_data[0] < 0xF0) {
      // 3 byte
      return ((utf8_data[0] & 0xF) << 12) + ((utf8_data[1] & 0x3F) << 6) + (utf8_data[2] & 0x3F);
    } else {
      // 4 byte
      return ((utf8_data[0] & 0x7) << 18) + ((utf8_data[1] & 0x3F) << 12) + ((utf8_data[2] & 0x3F) << 6) + (utf8_data[3] & 0x3F);
    }
  }

  pointer buffer;
  size_type reserved_size;
  size_type used_size;
};

#endif //UTF_FSTRING_H
