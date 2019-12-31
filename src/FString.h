//
// Created by bodguy on 2020/01/01.
//

#ifndef UTF_FSTRING_H
#define UTF_FSTRING_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ostream>

class FString {
  static const size_t initial_capacity = 32;
public:
  FString() : buffer(NULL), reserved_size(0L), used_size(0L) {
    grow_buffer(initial_capacity, false);
  }
  FString(const char* str) : buffer(NULL), reserved_size(0L), used_size(0L) {
    copy_string_to_buffer(str, strlen(str));
  }
  FString(const char c) : buffer(NULL), reserved_size(0L), used_size(0L) {
    grow_buffer(initial_capacity, false);
    buffer[0] = c;
    buffer[1] = '\0';
    used_size = 1;
  }
  FString(const FString& other) : buffer(NULL), reserved_size(0L), used_size(0L) {
    copy_string_to_buffer(other.buffer, other.used_size);
  }
  ~FString() {
    free(buffer);
  }

  size_t size() const {
    return used_size;
  }

  size_t length() const {
    return used_size;
  }

  size_t capacity() const {
    return reserved_size;
  }

  const char* c_str() const {
    return buffer;
  }

  FString& operator=(const FString& other) {
    if (this != &other) {
      copy_string_to_buffer(other.buffer, other.used_size);
    }

    return *this;
  }

  void reserve(size_t new_size) {
    grow_buffer(new_size, true);
  }

  FString& operator+=(const FString& other) {
    size_t total_size = used_size + other.used_size;
    if (total_size > reserved_size) {
      reserve(recommend_reserve_size(total_size));
    }
    strcat(buffer, other.buffer);
    used_size = total_size;
    buffer[used_size] = '\0';

    return *this;
  }

  char& operator[](size_t index) {
    return buffer[index];
  }

  const char& operator[](size_t index) const {
    return buffer[index];
  }

  char& at(size_t index) {
    assert(index < used_size);
    return buffer[index];
  }

  const char& at(size_t index) const {
    assert(index < used_size);
    return buffer[index];
  }

  bool empty() const {
    return used_size == 0;
  }

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
  void grow_buffer(size_t new_size, bool copy_data) {
    if (new_size > reserved_size) {
      char* new_buffer = (char*)malloc(new_size * sizeof(char));

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

  static size_t recommend_reserve_size(size_t str_len) {
    return (str_len + 1) * 2;
  }

  void copy_string_to_buffer(const char* str, size_t str_len) {
    if (str_len >= reserved_size) {
      grow_buffer(recommend_reserve_size(str_len), false);
    }
    used_size = str_len;
    memcpy(buffer, str, str_len);
    buffer[used_size] = '\0';
  }

private:
  char* buffer;
  size_t reserved_size;
  size_t used_size;
};


#endif //UTF_FSTRING_H
