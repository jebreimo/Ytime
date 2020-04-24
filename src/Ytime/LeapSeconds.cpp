//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-04-23.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ytime/LeapSeconds.hpp"
#include <algorithm>
#include <tuple>
#include "InternalDateTimeMath.hpp"

namespace Ytime
{
    constexpr std::tuple<PackedDateTime, uint32_t, uint32_t>
    makeLeapSecondTuple(DateTime dateTime, uint32_t leapSecs) noexcept
    {
        return {
            packInternalDateTime(dateTime),
            daysSinceEpochYMD(dateTime.date),
            leapSecs
        };
    }

    constexpr std::tuple<PackedDateTime, uint32_t, uint32_t> LEAP_SECONDS[] = {
        makeLeapSecondTuple({{1972, 7, 1}, {0, 0, 1}}, 1),
        makeLeapSecondTuple({{1973, 1, 1}, {0, 0, 2}}, 2),
        makeLeapSecondTuple({{1974, 1, 1}, {0, 0, 3}}, 3),
        makeLeapSecondTuple({{1975, 1, 1}, {0, 0, 4}}, 4),
        makeLeapSecondTuple({{1976, 1, 1}, {0, 0, 5}}, 5),
        makeLeapSecondTuple({{1977, 1, 1}, {0, 0, 6}}, 6),
        makeLeapSecondTuple({{1978, 1, 1}, {0, 0, 7}}, 7),
        makeLeapSecondTuple({{1979, 1, 1}, {0, 0, 8}}, 8),
        makeLeapSecondTuple({{1980, 1, 1}, {0, 0, 9}}, 9),
        makeLeapSecondTuple({{1981, 7, 1}, {0, 0, 10}}, 10),
        makeLeapSecondTuple({{1982, 7, 1}, {0, 0, 11}}, 11),
        makeLeapSecondTuple({{1983, 7, 1}, {0, 0, 12}}, 12),
        makeLeapSecondTuple({{1985, 7, 1}, {0, 0, 13}}, 13),
        makeLeapSecondTuple({{1988, 1, 1}, {0, 0, 14}}, 14),
        makeLeapSecondTuple({{1990, 1, 1}, {0, 0, 15}}, 15),
        makeLeapSecondTuple({{1991, 1, 1}, {0, 0, 16}}, 16),
        makeLeapSecondTuple({{1992, 7, 1}, {0, 0, 17}}, 17),
        makeLeapSecondTuple({{1993, 7, 1}, {0, 0, 18}}, 18),
        makeLeapSecondTuple({{1994, 7, 1}, {0, 0, 19}}, 19),
        makeLeapSecondTuple({{1996, 1, 1}, {0, 0, 20}}, 20),
        makeLeapSecondTuple({{1997, 7, 1}, {0, 0, 21}}, 21),
        makeLeapSecondTuple({{1999, 1, 1}, {0, 0, 22}}, 22),
        makeLeapSecondTuple({{2006, 1, 1}, {0, 0, 23}}, 23),
        makeLeapSecondTuple({{2009, 1, 1}, {0, 0, 24}}, 24),
        makeLeapSecondTuple({{2012, 7, 1}, {0, 0, 25}}, 25),
        makeLeapSecondTuple({{2015, 7, 1}, {0, 0, 26}}, 26),
        makeLeapSecondTuple({{2017, 1, 1}, {0, 0, 27}}, 27)
    };

    uint32_t getLeapSeconds(PackedDateTime dateTime)
    {
        using std::get;
        auto it = std::upper_bound(
            std::begin(LEAP_SECONDS), std::end(LEAP_SECONDS),
            std::tuple(dateTime, 0u, 0u),
            [](auto& a, auto& b) {return get<0>(a) < get<0>(b);});
        if (it == std::begin(LEAP_SECONDS))
            return 0;
        return get<2>(*prev(it));
    }

    bool isLeapSecond(PackedDateTime dateTime)
    {
        using std::get;
        auto it = std::lower_bound(
            std::begin(LEAP_SECONDS), std::end(LEAP_SECONDS),
            std::tuple(dateTime, 0u, 0u),
            [](auto& a, auto& b) {return get<0>(a) < get<0>(b);});
        if (it == std::end(LEAP_SECONDS))
            return false;
        return dateTime < get<0>(*it) && dateTime + USECS_PER_SEC >= get<0>(*it);
    }

    uint32_t getLeapSeconds(DateTime dateTime)
    {
        return getLeapSeconds(pack(dateTime));
    }

    bool isLeapSecond(DateTime dateTime)
    {
        return isLeapSecond(pack(dateTime));
    }

    uint32_t getLeapSeconds(YearMonthDay date)
    {
        using std::get;
        auto it = std::upper_bound(
            std::begin(LEAP_SECONDS), std::end(LEAP_SECONDS),
            std::tuple(PackedDateTime(0), daysSinceEpochYMD(date), 0u),
            [](auto& a, auto& b) {return get<1>(a) < get<1>(b);});
        if (it == std::begin(LEAP_SECONDS))
            return 0;
        return get<2>(*prev(it));
    }

    bool hasLeapSecond(YearMonthDay date)
    {
        using std::get;
        return std::binary_search(
            std::begin(LEAP_SECONDS), std::end(LEAP_SECONDS),
            std::tuple(PackedDateTime(0), daysSinceEpochYMD(date) + 1, 0u),
            [](auto& a, auto& b) {return get<1>(a) < get<1>(b);});
    }
}
