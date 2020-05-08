//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-04-23.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <algorithm>
#include <tuple>
#include <vector>
#include "Ytime/PackedDateTime.hpp"

namespace Ytime
{
    /* Internally, March is the first month. This simplifies handling the
       28/29 days of February.
     */
    constexpr uint32_t ACCUMULATED_DAYS[12] = {
        0, 31, 61, 92, 122, 153,
        184, 214, 245, 275, 306, 337};

    constexpr uint32_t daysSinceEpochY(uint32_t year) noexcept
    {
        auto years = year - EPOCH_YEAR;
        return years * 365 + years / 4 - years / 100 + years / 400;
    }

    constexpr uint32_t daysSinceEpochYMD(YearMonthDay date) noexcept
    {
        /* Make March the first month of the year. */
        if (date.month > 2)
        {
            date.month -= 3;
        }
        else
        {
            date.month += 9;
            --date.year;
        }
        return daysSinceEpochY(date.year)
               + ACCUMULATED_DAYS[date.month]
               + date.day - 1;
    }

    constexpr std::pair<uint32_t, uint32_t>
    toInternalYD(uint32_t daysSinceEpoch) noexcept
    {
        constexpr uint32_t days400 = 400 * 365 + 4 * 24 + 1; /* Number of days in 4 centuries. */
        constexpr uint32_t days100 =
            100 * 365 + 24; /* Number of days in a century not divisible by 400. */
        constexpr uint32_t days4 = 4 * 365 + 1; /* Number of days in 4 years. */

        auto n = daysSinceEpoch / days400;
        auto year = EPOCH_YEAR + 400 * n;
        daysSinceEpoch -= n * days400;

        n = std::min(3u, daysSinceEpoch / days100);
        year += 100 * n;
        daysSinceEpoch -= n * days100;

        n = daysSinceEpoch / days4;
        year += 4 * n;
        daysSinceEpoch -= n * days4;

        n = std::min(3u, daysSinceEpoch / 365);
        year += n;
        return {year, daysSinceEpoch - n * 365};
    }

    YearMonthDay toYMD(uint64_t daysSinceEpoch);

    constexpr HourMinuteSecond toHMS(uint64_t useconds) noexcept
    {
        auto hour = std::min(uint64_t(23), useconds / (60 * 60 * USECS_PER_SEC));
        useconds -= hour * 60 * 60 * USECS_PER_SEC;
        auto minute = std::min(uint64_t(59), useconds / (60 * USECS_PER_SEC));
        useconds -= minute * 60 * USECS_PER_SEC;
        auto second = useconds / USECS_PER_SEC;
        auto usecond = useconds % USECS_PER_SEC;
        return {int(hour), int(minute), int(second), int(usecond)};
    }

    constexpr uint64_t usecsSinceMidnight(HourMinuteSecond time) noexcept
    {
        return time.hour * USECS_PER_HOUR + time.minute * USECS_PER_MIN
               + time.second * USECS_PER_SEC + time.usecond;
    }

    constexpr PackedDateTime
    packDaysUseconds(uint64_t days, uint64_t usecs) noexcept
    {
        return PackedDateTime(days * USECS_PER_DAY + usecs);
    }

    constexpr std::pair<uint64_t, uint64_t>
    unpackDaysUseconds(PackedDateTime dateTime) noexcept
    {
        return {dateTime / USECS_PER_DAY, dateTime % USECS_PER_DAY};
    }

    constexpr PackedDateTime packInternalDateTime(const DateTime& dateTime) noexcept
    {
        auto days = daysSinceEpochYMD(dateTime.date);
        auto usecs = usecsSinceMidnight(dateTime.time);
        return packDaysUseconds(days, usecs);
    }
}
