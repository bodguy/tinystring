#include <codecvt>
#include <vector>
#include <utf8/utf8.h>
#include "String.h"

String::String() : _str() {}
String::String(const std::string& str) : _str(utf8::utf8to16(str)) {}
String::String(const std::u32string& str) : _str(utf8::utf8to16(utf8::utf32to8(str))) {}
String::String(const std::u16string& str) : _str(str) {}
String::String(const char* byte) : _str(utf8::utf8to16(byte)) {}
String::String(const char16_t* byte) : _str(byte) {}
String::String(const char32_t* byte) : _str(utf8::utf8to16(utf8::utf32to8(byte))) {}
String::String(const String& str) :_str(str._str) {}

const char16_t* String::c16_str() const {
  return _str.c_str();
}

size_t String::length() const { return _str.size(); }

size_t String::size() const { return _str.size(); }

bool String::empty() const {
  return _str.empty();
}

void String::clear() {
  _str.clear();
}

String String::substr(size_t pos, size_t len) const {
  if (pos == len) {
    return String();
  }

  if (pos > len) {
    throw std::out_of_range("pos should not be greater than len");
  }

  return String(_str.substr(pos, len));
}

std::string String::to_string() const {
  return utf8::utf16to8(_str);
}

double String::to_double() const {
  return std::stod(utf8::utf16to8(_str));
}

float String::to_float() const {
  return std::stof(utf8::utf16to8(_str));
}

int String::to_int() const {
  return std::stoi(utf8::utf16to8(_str));
}

bool String::contains(const String& contain) const {
  return (_str.find(contain._str) != std::u16string::npos);
}

bool String::starts_with(const String& prefix) const {
  return (_str.length() >= prefix._str.length()
      && _str.compare(0, prefix._str.length(), prefix._str) == 0);
}

bool String::ends_with(const String& suffix) const {
  return (_str.length() >= suffix._str.length()
      && _str.compare(_str.length() - suffix._str.length(), suffix._str.length(), suffix._str) == 0);
}

bool String::equals_ignore_case(const String& compare) const {
  size_t length = compare._str.size();
  if (_str.size() != length) {
    return false;
  }

  for (size_t i = 0; i < length; i++) {
    if (std::tolower(_str[i]) != tolower(compare._str[i])) {
      return false;
    }
  }

  return true;
}

String String::replace_all(const String& from, const String& to) const {
  std::u16string result = _str;
  std::u16string::size_type pos = 0;
  std::u16string::size_type offset = 0;

  while ((pos = result.find(from._str, offset)) != std::u16string::npos) {
    result.replace(result.begin() + pos, result.begin() + pos + from._str.length(), to._str);
    offset = pos + to.length();
  }

  return String(result);
}

String& String::operator=(const String& str) {
  if (this != &str) {
    _str = str._str;
  }

  return *this;
}

String& String::operator=(const std::string& str) {
  _str = utf8::utf8to16(str);
  return *this;
}

String& String::operator=(const std::u32string& str) {
  _str = utf8::utf8to16(utf8::utf32to8(str));
  return *this;
}

String& String::operator=(const std::u16string& str) {
  _str = str;
  return *this;
}

String& String::operator=(const char* str) {
  _str = utf8::utf8to16(str);
  return *this;
}

String& String::operator=(const char16_t* str) {
  _str = str;
  return *this;
}

String& String::operator=(const char32_t* str) {
  _str = utf8::utf8to16(utf8::utf32to8(str));
  return *this;
}

bool String::operator==(const String& other) const {
  return _str == other._str;
}

bool String::operator!=(const String& other) const {
  return !(*this == other);
}

String& String::operator+=(const String& str) {
  _str += str._str;
  return *this;
}

String& String::operator+=(const char* s) {
  _str.append(utf8::utf8to16(s));
  return *this;
}

String& String::operator+=(const char16_t* s) {
  _str.append(s);
  return *this;
}

char16_t& String::operator[](size_t pos) {
  return _str[pos];
}

const char16_t& String::operator[](size_t pos) const {
  return _str[pos];
}

std::u16string::iterator String::begin() {
  return _str.begin();
}

std::u16string::iterator String::end() {
  return _str.end();
}

std::u16string::const_iterator String::cbegin() const {
  return _str.cbegin();
}

std::u16string::const_iterator String::cend() const {
  return _str.cend();
}

std::u16string::reverse_iterator String::rbegin() {
  return _str.rbegin();
}

std::u16string::reverse_iterator String::rend() {
  return _str.rend();
}

std::ostream& operator<<(std::ostream& os, const String& str) {
  os << str.to_string();
  return os;
}

std::ostream& operator<<(std::ostream& os, const char16_t& s) {
  static std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> convert;
  os << convert.to_bytes(s);
  return os;
}

String operator+(const String& lhs, const String& rhs) {
  return String(lhs._str + rhs._str);
}

String operator+(const String& lhs, const char16_t* rhs) {
  return String(lhs._str + std::u16string(rhs));
}

String operator+(const char16_t* lhs, const String& rhs) {
  return String(std::u16string(lhs) + rhs._str);
}
