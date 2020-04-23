//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-04-22.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ytime/PackedDateTime.hpp"
#include <catch2/catch.hpp>

void checkDelta(Ytime::DateTime from, Ytime::DateTime to, Ytime::Delta delta)
{
    CAPTURE(from, to);
    auto actualDelta = getDeltaDays(pack(from), pack(to));
    REQUIRE(actualDelta == delta);
}

using namespace Ytime;

TEST_CASE("From leap second, delta is 0")
{
    checkDelta({{2015, 6, 30}, {23, 59, 60}},
               {{2015, 6, 30}, {23, 59, 60}},
               {Days(0), Seconds(0)});
}

TEST_CASE("End after leap second")
{
    checkDelta({{2015, 6, 29}, {23, 59, 59}},
               {{2015, 7, 1}, {0, 0, 0}},
               {Days(1), Seconds(2)});
}

TEST_CASE("Exactly one day, ends after leap second")
{
    checkDelta({{2015, 6, 30}, {0, 0, 0}},
               {{2015, 7, 1}, {0, 0, 0}},
               {Days(1), Seconds(0)});
}

TEST_CASE("Positive delta, 86400 on leap second")
{
    checkDelta({{2015, 6, 30}, {0, 0, 0}},
               {{2015, 6, 30}, {23, 59, 60}},
               {Days(0), Seconds(86400)});
}

TEST_CASE("Negative delta, -86400 on leap second")
{
    checkDelta({{2015, 7, 1}, {23, 59, 59}},
               {{2015, 6, 30}, {23, 59, 60}},
               {Days(0), Seconds(-86400)});
}

TEST_CASE("Negative delta, end on leap second.")
{
    checkDelta({{2015, 7, 2}, {0, 0, 0}},
               {{2015, 6, 30}, {23, 59, 60}},
               {Days(-1), Seconds(-1)});
}

TEST_CASE("Negative delta, after to before leap second.")
{
    checkDelta({{2015, 7, 2}, {0, 0, 0}},
               {{2015, 6, 30}, {0, 0, 0}},
               {Days(-2), Seconds(0)});
}

TEST_CASE("Positive delta, before second leap second.")
{
    checkDelta({{2015, 6, 30}, {23, 59, 59}},
               {{2016, 12, 31}, {23, 59, 59}},
               {Days(550), Seconds(0)});
}

TEST_CASE("Positive delta, on second leap second.")
{
    checkDelta({{2015, 6, 30}, {23, 59, 59}},
               {{2016, 12, 31}, {23, 59, 60}},
               {Days(550), Seconds(1)});
}

TEST_CASE("Positive delta, after second leap second.")
{
    checkDelta({{2015, 6, 30}, {23, 59, 59}},
               {{2017, 1, 1}, {0, 0, 0}},
               {Days(550), Seconds(2)});
}

TEST_CASE("Test delta days near leap seconds")
{
    checkDelta({{2015, 6, 30}, {23, 00, 00}},
               {{2015, 6, 30}, {23, 59, 59}},
               {Days(0), Seconds(3599)});
    checkDelta({{2015, 6, 30}, {23, 00, 00}},
               {{2015, 6, 30}, {23, 59, 60}},
               {Days(0), Seconds(3600)});
    checkDelta({{2015, 6, 30}, {23, 00, 00}},
               {{2015, 7, 1}, {22, 59, 59}},
               {Days(0), Seconds(86400)});
    checkDelta({{2015, 6, 30}, {23, 00, 00}},
               {{2015, 7, 1}, {23, 00, 00}},
               {Days(1), Seconds(0)});
    checkDelta({{2015, 7, 1}, {22, 59, 59}},
               {{2015, 6, 30}, {23, 00, 00}},
               {Days(0), Seconds(-86400)});

    checkDelta({{2015, 6, 30}, {23, 59, 59}},
               {{2015, 6, 30}, {23, 59, 59}},
               {Days(0), Seconds(0)});
    checkDelta({{2015, 6, 30}, {23, 59, 59}},
               {{2015, 6, 30}, {23, 59, 60}},
               {Days(0), Seconds(1)});
    checkDelta({{2015, 6, 30}, {23, 59, 59}},
               {{2015, 7, 1}, {23, 59, 59}},
               {Days(1), Seconds(0)});
    checkDelta({{2015, 6, 29}, {23, 59, 59}},
               {{2015, 7, 1}, {23, 59, 59}},
               {Days(2), Seconds(0)});
    checkDelta({{2015, 6, 29}, {23, 59, 59}},
               {{2015, 6, 30}, {23, 59, 59}},
               {Days(1), Seconds(0)});
    checkDelta({{2015, 6, 29}, {23, 59, 59}},
               {{2015, 6, 30}, {23, 59, 60}},
               {Days(1), Seconds(1)});
    checkDelta({{2015, 6, 30}, {0, 0, 0}},
               {{2015, 7, 1}, {0, 0, 0}},
               {Days(1), Seconds(0)});

    checkDelta({{2015, 7, 1}, {23, 59, 59}},
               {{2015, 6, 29}, {23, 59, 59}},
               {Days(-2), Seconds(0)});
    checkDelta({{2015, 7, 1}, {23, 59, 59}},
               {{2015, 6, 30}, {23, 59, 60}},
               {Days(0), Seconds(-86400)});
}
