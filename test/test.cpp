#include <Catch2/catch.hpp>
#include <string>

TEST_CASE("StringUtil unit test") {
  SECTION("string contains") {
    std::string dest = "#include";
    REQUIRE(dest == "#include");
  }
}