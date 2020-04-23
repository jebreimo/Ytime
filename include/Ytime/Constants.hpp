//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-04-23.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <cstdint>

namespace Ytime
{
    constexpr uint32_t SECS_PER_DAY = 24ULL * 60ULL * 60ULL;
    constexpr uint64_t USECS_PER_SEC = 1000000ULL;
    constexpr uint64_t USECS_PER_MIN = 60 * USECS_PER_SEC;
    constexpr uint64_t USECS_PER_HOUR = 60 * USECS_PER_MIN;
    constexpr uint64_t USECS_PER_DAY = SECS_PER_DAY * USECS_PER_SEC;

    /* The epoch year is the year that has day 0 in the date calculations.
       This must be a leap century and must be prior to the first year that is
       "officially" supported by these functions.
       1200 is the last leap-century before the Gregorian calendar was introduced
       in October 1582. */
    constexpr uint32_t EPOCH_YEAR = 1200;
    constexpr uint32_t MIN_YEAR = 1582;
}
