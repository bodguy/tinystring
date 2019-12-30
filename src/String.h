#ifndef STRING_H
#define STRING_H

#include <string>
#include <ostream>

// represents UTF-16 string
class String {
public:
  String();
  String(const std::string& str);
  String(const std::u32string& str);
  String(const std::u16string& str);
  String(const char* byte);
  String(const char16_t* byte);
  String(const char32_t* byte);
  String(const String& str);

  const char16_t* c16_str() const;
  size_t length() const;
  size_t size() const;
  bool empty() const;
  void clear();
  String substr(size_t pos = 0, size_t len = std::string::npos) const;
  std::string to_string() const;
  double to_double() const;
  float to_float() const;
  int to_int() const;
  bool contains(const String& contain) const;
  bool starts_with(const String& prefix) const;
  bool ends_with(const String& suffix) const;
  bool equals_ignore_case(const String& compare) const;
  String replace_all(const String& from, const String& to) const;

  String& operator=(const String& str);
  String& operator=(const std::string& str);
  String& operator=(const std::u32string& str);
  String& operator=(const std::u16string& str);
  String& operator=(const char* str);
  String& operator=(const char16_t* str);
  String& operator=(const char32_t* str);

  bool operator==(const String& other) const;
  bool operator!=(const String& other) const;
  String& operator+=(const String& str);
  String& operator+=(const char* s);
  String& operator+=(const char16_t* s);
  char16_t& operator[](size_t pos);
  const char16_t& operator[](size_t pos) const;

  std::u16string::iterator begin();
  std::u16string::iterator end();
  std::u16string::const_iterator cbegin() const;
  std::u16string::const_iterator cend() const;
  std::u16string::reverse_iterator rbegin();
  std::u16string::reverse_iterator rend();

  friend std::ostream& operator<<(std::ostream& os, const String& str);
  friend String operator+(const String& lhs, const String& rhs);
  friend String operator+(const String& lhs, const char16_t* rhs);
  friend String operator+(const char16_t* lhs, const String& rhs);

private:
  std::u16string _str;
};

std::ostream& operator<<(std::ostream& os, const String& str);
std::ostream& operator<<(std::ostream& os, const char16_t& s);
String operator+(const String& lhs, const String& rhs);
String operator+(const String& lhs, const char16_t* rhs);
String operator+(const char16_t* lhs, const String& rhs);

#endif // STRING_H
