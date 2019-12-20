#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <codecvt>   // for codecvt_utf8
#include <locale>    // for wstring_convert
#include <cstring>
#include "utf8.h"
#include "String.h"

using namespace std;

int main(int argc, char** argv) {
//  const char* test_file_path = "../sample8.json";
//  ifstream fs8(test_file_path);
//  if (!fs8.is_open()) {
//    cout << "Could not open " << test_file_path << endl;
//    return 0;
//  }
//
//  unsigned line_count = 1;
//  string line;
//  int total_length = 0;
//  u16string utf16line;
//  string utf8line;
//  wstring_convert<codecvt_utf8<char16_t>, char16_t> cv;
//
//  while (getline(fs8, line)) {
//    auto end_it = utf8::find_invalid(line.begin(), line.end());
//    if (end_it != line.end()) {
//      cout << "Invalid UTF-8 encoding detected at line " << line_count << "\n";
//      cout << "This part is fine: " << string(line.begin(), end_it) << "\n";
//    }
//
//    int length = utf8::distance(line.begin(), end_it);
//    total_length += length;
//
//    utf16line = utf8::utf8to16(line);
//    utf8line = utf8::utf16to8(utf16line);
//    cout << "utf8 len: " << utf8line.size() << "\n";
//    cout << utf8line << "\n";
//
//
//    const char* arr = utf8::utf16to8(utf16line).c_str();
//    size_t len = strlen(arr);
//    cout << "strlen: " << len << "\n";
//    printf("utf16 rune: %s\n", arr);
//
//    if (utf8line != string(line.begin(), end_it))
//      cout << "Error in UTF-16 conversion at line: " << line_count << "\n";
//
//    line_count++;
//  }
//
//  cout << "total length: " << total_length << "\n";
//  cout << utf16line.size() << "\n";

//  int i = 0;
//  for (char16_t c : utf16line) {
//    cout << "utf1[" << i << "]: " << cv.to_bytes(c) << "\n";
//    i++;
//  }

  u16string str2 = u"한글";
//  str2.append(u"입니다");
//  cout << "this: " << utf8::utf16to8(str2).c_str() << endl;
  printf("%s\n", utf8::utf16to8(str2).c_str());

  String str(u16string(u"한글"));
  u16string str3 = str.getStr();
  printf("%s\n", utf8::utf16to8(str3).c_str());


//  if (utf16line == str2) {
//    cout << "same" << "\n";
//  } else {
//    cout << "not same" << "\n";
//  }

//  int j = 0;
//  for (char16_t c : str2) {
//    cout << "utf1[" << j << "]: " << cv.to_bytes(c) << "\n";
//    j++;
//  }
//
//  u16string ss = u"16.22";
//  double d = std::stod(utf8::utf16to8(ss));
//  cout << d << "\n";
//
//  String str(u16string(u"hello"));
//  printf("%s\n", str.c_str());

  return 0;
}