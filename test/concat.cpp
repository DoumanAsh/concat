#include <catch.hpp>

#include <vector>
#include <iostream>
#include <cassert>
#include <sstream>

#include <concat.hpp>

struct Lolka {};

inline std::ostream& operator<<(std::ostream& out, const Lolka& lolka) {
    (void)(lolka);
    out << "lolka";
    return out;
}

TEST_CASE("try simple concat") {
    std::stringstream result;
    result << concat::concat("test", '=', concat::concat(Lolka()).quote('"').quote("-"));
    REQUIRE(result.str() == "test=-\"lolka\"-");
}

TEST_CASE("try joined concat") {
    std::stringstream result;
    result << concat::concat("test", '=', 1).join(" ");
    REQUIRE(result.str() == "test = 1");
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
