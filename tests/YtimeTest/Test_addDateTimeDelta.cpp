//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-04-24.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ytime/PackedDateTime.hpp"
#include <catch2/catch.hpp>

using namespace Ytime;

namespace
{
    void checkSum(DateTime from, DateTimeDelta delta, DateTime expected)
    {
        CAPTURE(from, delta);
        auto packedTo = add(pack(from), delta);
        auto to = unpack(packedTo);
        REQUIRE(to == expected);
    }
}

TEST_CASE("Across leap seconds")
{
    checkSum({{1990, 4, 24}, {9, 45, 0}},
             Days(10958),
             {{2020, 4, 24}, {9, 45, 0}});
}

TEST_CASE("End on leap second from 23:59:59")
{
    checkSum({{1990, 12, 31}, {23, 59, 59}},
             Days(9497),
             {{2016, 12, 31}, {23, 59, 59}});
    checkSum({{1990, 12, 31}, {23, 59, 59}},
             Days(9497) + Seconds(1),
             {{2016, 12, 31}, {23, 59, 60}});
    checkSum({{1990, 12, 31}, {23, 59, 59}},
             Days(9497) + Seconds(2),
             {{2017, 1, 1}, {0, 0, 0}});
}

TEST_CASE("End on leap second from 00:00:00")
{
    checkSum({{1990, 12, 31}, {0, 0, 0}},
             Days(9497) + Seconds(86399),
             {{2016, 12, 31}, {23, 59, 59}});
    checkSum({{1990, 12, 31}, {0, 0, 0}},
             Days(9497) + Seconds(86400),
             {{2016, 12, 31}, {23, 59, 60}});
    checkSum({{1990, 12, 31}, {0, 0, 0}},
             Days(9498),
             {{2017, 1, 1}, {0, 0, 0}});
}

TEST_CASE("Negative, across leap seconds")
{
    checkSum({{2020, 4, 24}, {9, 45, 0}},
             Days(-10958),
             {{1990, 4, 24}, {9, 45, 0}});
}

TEST_CASE("Negative, end on leap second from 23:59:59")
{
    checkSum({{2019, 12, 31}, {23, 59, 59}},
             Days(-7670),
             {{1998, 12, 31}, {23, 59, 59}});
    checkSum({{2019, 12, 31}, {23, 59, 59}},
             Days(-7669) + Seconds(-86400),
             {{1998, 12, 31}, {23, 59, 60}});
    checkSum({{2019, 12, 31}, {23, 59, 59}},
             Days(-7669) + Seconds(-86399),
             {{1999, 1, 1}, {0, 0, 0}});
}
