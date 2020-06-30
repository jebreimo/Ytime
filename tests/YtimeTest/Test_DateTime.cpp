//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-06-30.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ytime/DateTime.hpp"

#include <catch2/catch.hpp>

TEST_CASE("Test parseTime")
{
    using namespace Ytime;
    auto t = parseTime("13:12:10.678");
    REQUIRE(t);
    REQUIRE(*t == Time(13, 12, 10, 678000));
}
