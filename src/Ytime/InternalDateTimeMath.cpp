//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-04-23.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "InternalDateTimeMath.hpp"

namespace Ytime
{
    YearMonthDay toYMD(uint64_t daysSinceEpoch)
    {
        auto[year, dayOfYear] = toInternalYD(daysSinceEpoch);
        auto it = std::upper_bound(std::begin(ACCUMULATED_DAYS),
                                   std::end(ACCUMULATED_DAYS),
                                   dayOfYear);
        auto month = std::distance(std::begin(ACCUMULATED_DAYS), it);
        auto dayOfMonth = dayOfYear - ACCUMULATED_DAYS[month - 1] + 1;
        if (month > 10)
        {
            month -= 10;
            ++year;
        }
        else
        {
            month += 2;
        }
        return {int(year), int(month), int(dayOfMonth)};
    }
}
