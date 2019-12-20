#ifndef STRING_H
#define STRING_H

#include <string>
#include <ostream>
#include "utf8.h"

typedef char32_t rune;
typedef std::string u8string;

// represents UTF-16 string
class String {
public:
  String() : _str() {}
  explicit String(const std::string& str) : _str(utf8::utf8to16(str)){}
  explicit String(const std::u32string& str) : _str() {
    u8string u8 = utf8::utf32to8(str);
    _str = std::u16string(reinterpret_cast<const char16_t*>(u8.c_str()), u8.length()/sizeof(char16_t));
  }
  explicit String(const std::u16string& str) : _str() { std::copy(str.begin(), str.end(), _str); }
  explicit String(const char* byte) : _str(utf8::utf8to16(u8string(byte))) {}
  explicit String(const char16_t* byte) : _str(byte) {}

  const char* c_str() const {
    return utf8::utf16to8(_str).c_str();
  }

  const char16_t* c16_str() const {
    return _str.c_str();
  }

//  friend std::ostream& operator<<(std::ostream& os, const String& str);

private:
  std::u16string _str;
};

//std::ostream& operator<<(std::ostream& os, const String& str) {
//  os << str.c_str();
//  return os;
//}

#endif // STRING_H
