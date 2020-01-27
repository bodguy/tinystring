//
// Created by bodguy on 2020/01/01.
//

#ifndef UTF_STR_H
#define UTF_STR_H

#include "types.h"
#include <ostream>

template <typename T>
class Iter : public std::iterator<std::bidirectional_iterator_tag, T> {
private:
  typedef size_t size_type;
  typedef typename std::iterator<std::bidirectional_iterator_tag, T>::pointer pointer;
  typedef typename std::iterator<std::bidirectional_iterator_tag, T>::value_type value_type;
  typedef typename std::iterator<std::bidirectional_iterator_tag, T>::difference_type difference_type;
  typedef typename std::iterator<std::bidirectional_iterator_tag, T>::reference reference;

public:
  Iter(pointer ptr, size_type start);
  Iter(pointer ptr);
  FORCE_INLINE reference operator*() const;
  FORCE_INLINE Iter& operator++();
  FORCE_INLINE Iter operator++(int);
  FORCE_INLINE Iter& operator--();
  FORCE_INLINE Iter operator--(int);
  FORCE_INLINE bool operator == (const Iter& other) const;
  FORCE_INLINE bool operator != (const Iter& other) const;
  FORCE_INLINE bool operator < (const Iter& other) const;
  FORCE_INLINE bool operator > (const Iter& other) const;
  FORCE_INLINE bool operator <= (const Iter& other) const;
  FORCE_INLINE bool operator >= (const Iter& other) const;

private:
  FORCE_INLINE void inc();
  FORCE_INLINE void dec();

  pointer buf_;
};

class Str {
public:
  friend std::ostream& operator <<(std::ostream& os, const Str& str);
  friend Str operator +(const Str& lhs, const Str& rhs);

  typedef u16 value_type;
  typedef size_t size_type;
  typedef value_type* pointer;
  typedef value_type& reference;
  typedef Iter<value_type> iterator;
  typedef Iter<const value_type> const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

public:
  Str();
  Str(const pointer str);
  Str(const i8* str);
  Str(const value_type c);
  Str(const Str& other);
  ~Str();

  FORCE_INLINE size_type size() const;
  FORCE_INLINE size_type length() const;
  FORCE_INLINE size_type capacity() const;
  FORCE_INLINE void reserve(size_type new_size);
  FORCE_INLINE Str::pointer c_str() const;
  FORCE_INLINE Str& operator =(const Str& other);
  FORCE_INLINE Str& operator +=(const Str& other);
  FORCE_INLINE bool operator ==(const Str& other) const;
  FORCE_INLINE reference operator[](size_type index);
  FORCE_INLINE reference operator[](size_type index) const;
  FORCE_INLINE reference at(size_type index);
  FORCE_INLINE reference at(size_type index) const;
  FORCE_INLINE bool empty() const;

  FORCE_INLINE size_type codepoint_count(usize begin, usize end) const;
  FORCE_INLINE std::string to_utf8_str() const;
  FORCE_INLINE std::string to_utf8_char(u32 cp) const;

  FORCE_INLINE iterator begin();
  FORCE_INLINE iterator end();
  FORCE_INLINE const_iterator cbegin() const;
  FORCE_INLINE const_iterator cend() const;

private:
  static size_type recommend_reserve_size(size_type str_len);
  static const size_type initial_capacity;
  typedef u8 utf8_encoding[4];

  FORCE_INLINE void grow_buffer(size_type new_size, bool copy_data);
  FORCE_INLINE void copy_string_to_buffer(const pointer str, size_type str_len);
  FORCE_INLINE void get_utf8_encoding(u32 in, utf8_encoding& out, int& out_size, bool default_order = true);
  FORCE_INLINE void get_utf8_encoding(u16 in, utf8_encoding& out, int& out_size, bool default_order = true);
  FORCE_INLINE u16 from_utf8(const u8* v) const;
  FORCE_INLINE u16 from_utf8_lossy(const u8* v) const;
  template <typename T> FORCE_INLINE u16 mask16(T oc) const;
  template <typename T> FORCE_INLINE bool is_lead_surrogate(T cp) const;

  pointer buffer;
  size_type reserved_size;
  size_type used_size;
};

template <typename T>
Iter<T>::Iter(pointer ptr, size_type start) {
  buf_ = &ptr[start];
}

template <typename T>
Iter<T>::Iter(pointer ptr) : buf_(ptr) {}

template <typename T>
typename Iter<T>::reference Iter<T>::operator *() const {
  return *buf_;
}

template <typename T>
Iter<T>& Iter<T>::operator ++() {
  inc(); return *this;
}

template <typename T>
Iter<T> Iter<T>::operator ++(int) {
  Iter copy(*this);
  inc();
  return copy;
}

template <typename T>
Iter<T>& Iter<T>::operator --() {
  dec(); return *this;
}

template<typename T>
Iter<T> Iter<T>::operator --(int) {
  Iter copy(*this);
  dec();
  return copy;
}

template<typename T>
bool Iter<T>::operator ==(const Iter& other) const {
  return buf_ == other.buf_;
}

template<typename T>
bool Iter<T>::operator !=(const Iter& other) const {
  return buf_ != other.buf_;
}

template<typename T>
bool Iter<T>::operator <(const Iter& other) const {
  return buf_ < other.buf_;
}

template<typename T>
bool Iter<T>::operator >(const Iter& other) const {
  return buf_ > other.buf_;
}

template<typename T>
bool Iter<T>::operator <=(const Iter& other) const {
  return buf_ <= other.buf_;
}

template<typename T>
bool Iter<T>::operator >=(const Iter& other) const {
  return buf_ >= other.buf_;
}

template<typename T>
void Iter<T>::inc() {
  ++buf_;
}

template<typename T>
void Iter<T>::dec() {
  --buf_;
}

template <typename T>
u16 Str::mask16(T oc) const {
  return static_cast<u16>(0xFFFF & oc);
}

template <typename T>
bool Str::is_lead_surrogate(T cp) const {
  return (cp >= SUR_LEAD_START && cp <= SUR_LEAD_END);
}

#endif //UTF_STR_H
