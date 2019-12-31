#include <Catch2/catch.hpp>
#include <cstring>
#include "../src/FString.h"

TEST_CASE("FString unit test") {
  FString dest = "hello";
  FString other = " world";

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
  SECTION("is actually concat two string") {
    FString out = dest + other;
    REQUIRE(0 == strcmp(out.c_str(), "hello world"));
  }
  SECTION("concat with += operator") {
    dest += other;
    REQUIRE(0 == strcmp(dest.c_str(), "hello world"));
    REQUIRE(((strlen("hello") + 1) * 2) == dest.capacity());
  }
}