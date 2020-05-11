/* JEBBase: generic algorithms and functions
 * Copyright 2014 Jan Erik Breimo
 * All rights reserved.
 *
 * This file is distributed under the BSD License.
 * License text is included with the source distribution.
 */
#include "Ytime/PackedDateTime.hpp"

#include <algorithm>
#include "Ytime/LeapSeconds.hpp"
#include "InternalDateTimeMath.hpp"
#include "YtimeThrow.hpp"

namespace Ytime
{
    namespace
    {
        std::pair<uint64_t, uint64_t>
        unpackDaysUsecondsUtc(PackedDateTime dateTime) noexcept
        {
            auto leapsecs = getLeapSeconds(dateTime);
            auto secs = PackedDateTime(dateTime - leapsecs * USECS_PER_SEC);
            auto dayUsecs = unpackDaysUseconds(secs);
            if (isLeapSecond(dateTime))
            {
                --dayUsecs.first;
                dayUsecs.second += USECS_PER_DAY;
            }
            return dayUsecs;
        }
    }

    PackedDateTime pack(const DateTime& dateTime) noexcept
    {
        auto days = daysSinceEpochYMD(dateTime.date);
        auto usecs = usecsSinceMidnight(dateTime.time);
        return PackedDateTime(packDaysUseconds(days, usecs)
                              + getLeapSeconds(dateTime.date) * USECS_PER_SEC);
    }

    DateTime unpack(PackedDateTime dateTime) noexcept
    {
        auto daysUsecs = unpackDaysUsecondsUtc(dateTime);
        return {toYMD(daysUsecs.first), toHMS(daysUsecs.second)};
    }

    Date unpackDate(PackedDateTime dateTime) noexcept
    {
        return toYMD(unpackDaysUsecondsUtc(dateTime).first);
    }

    Time unpackTime(PackedDateTime dateTime) noexcept
    {
        return toHMS(unpackDaysUsecondsUtc(dateTime).second);
    }

    DateTimeDelta getDateTimeDelta(PackedDateTime from, PackedDateTime to)
    {
        if (from == to)
            return {};
        if (isLeapSecond(from))
            YTIME_THROW("Can not count days from a leap second.");
        auto delta = int64_t(to - from);
        auto toLS = int64_t(getLeapSeconds(to));
        auto fromLS = int64_t(getLeapSeconds(from));
        auto deltaLS = toLS - fromLS;
        delta -= deltaLS * int64_t(USECS_PER_SEC);
        auto days = delta / int64_t(USECS_PER_DAY);

        auto to0 = int64_t(from) + days * int64_t(USECS_PER_DAY);
        auto to0LS = int64_t(getLeapSeconds(PackedDateTime(to0)));
        to0 += (to0LS - fromLS) * int64_t(USECS_PER_SEC);
        auto usecs = int64_t(to) - to0;
        if (isLeapSecond(PackedDateTime(to0)))
        {
            if (usecs != 0)
            {
                usecs -= USECS_PER_SEC;
            }
            else if (days > 0)
            {
                --days;
                usecs = USECS_PER_DAY;
            }
            else
            {
                ++days;
                usecs = -int64_t(USECS_PER_DAY);
            }
        }
        return {days, usecs};
    }

    PackedDateTime add(PackedDateTime from, DateTimeDelta delta)
    {
        if (delta.days() == 0)
            return PackedDateTime(from + delta.totalUseconds());

        if (isLeapSecond(from))
            YTIME_THROW("Can not count days from a leap second.");
        auto to = from + delta.days() * int64_t(USECS_PER_DAY);
        auto toLS = int64_t(getLeapSeconds(PackedDateTime(to)));
        auto fromLS = int64_t(getLeapSeconds(from));
        to += (toLS - fromLS) * int64_t(USECS_PER_SEC);
        if (isLeapSecond(PackedDateTime(to)))
        {
            if (delta.days() > 0)
                to += USECS_PER_SEC;
            else
                to -= USECS_PER_SEC;
        }
        to += delta.totalUseconds();
        return PackedDateTime(to);
    }
}
