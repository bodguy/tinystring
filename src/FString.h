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

  pointer buffer;
  size_type reserved_size;
  size_type used_size;
};

#endif //UTF_FSTRING_H
