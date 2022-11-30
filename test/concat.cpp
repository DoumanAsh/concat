#include <catch.hpp>

#include <vector>
#include <iostream>
#include <cassert>
#include <sstream>

#include <concat.hpp>

TEST_CASE("try simple concat") {
    std::stringstream result;
    result << concat::concat("test", '=', 1);
    REQUIRE(result.str() == "test=1");
}

TEST_CASE("try from lvalue tuple") {
    std::stringstream result;
    auto res = std::make_tuple("test", '=', 1);
    result << concat::concat(res);
    REQUIRE(result.str() == "test=1");
}

TEST_CASE("try from rvalue tuple") {
    std::stringstream result;
    result << concat::concat(std::make_tuple("test", '=', 1));
    REQUIRE(result.str() == "test=1");
}
