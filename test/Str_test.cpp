#include <Catch2/catch.hpp>
#include <cstring>
#include "../src/Str.h"

TEST_CASE("FString unit test") {
  Str dest = "hello";
  Str other = " world";

  SECTION("string initialize with c string") {
    REQUIRE(0 == strcmp(dest.c_str(), "hello"));
  }
  SECTION("string actually has a recommended capacity size") {
    REQUIRE(((strlen("hello") + 1) * 2) == dest.capacity());
  }
  SECTION("string at does not throw any assertion") {
    REQUIRE('e' == dest.at(1));
  }
  SECTION("string operator [] can exceed buffer out of bound") {
    REQUIRE(0 == dest[dest.size()]);
  }
  SECTION("concat with += operator") {
    dest += other;
    REQUIRE(0 == strcmp(dest.c_str(), "hello world"));
    REQUIRE(((strlen("hello") + 1) * 2) == dest.capacity());
  }
}