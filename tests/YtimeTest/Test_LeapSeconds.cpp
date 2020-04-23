//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-04-23.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ytime/LeapSeconds.hpp"
#include <catch2/catch.hpp>

TEST_CASE("getLeapSeconds for DateTime")
{
    using namespace Ytime;
    REQUIRE(getLeapSeconds({{2017, 1, 1}, {0, 0, 0}}) == 27);
}

TEST_CASE("isLeapSecond for DateTime")
{
    using namespace Ytime;
    REQUIRE(isLeapSecond({{2016, 12, 31}, {23, 59, 60}}));
}
