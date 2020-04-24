/* JEBBase: generic algorithms and functions
 * Copyright 2014 Jan Erik Breimo
 * All rights reserved.
 *
 * This file is distributed under the BSD License.
 * License text is included with the source distribution.
 */
#pragma once

#include <cstdint>
#include <ctime>
#include <iosfwd>
#include <optional>
#include <string>
#include <utility>
#include "DateTime.hpp"
#include "DateTimeDelta.hpp"
#include "YtimeException.hpp"

/** @file This file defines a memory efficient representation of
    Gregorian dates and time. The resolution for time is microseconds
    and years range from 1200 to more than 500,000.

    PackedDateTime has several advantages, first it has no invalid values,
    every value maps to a unique date and time; second is it's a 64-bit integer
    so all operators that apply to integers also applies to dates and times,
    in particular assignment, comparison, addition´and subtraction operators;
    third, all these operations are very fast.
*/

namespace Ytime
{
    /** Type representing the number of micro-seconds since the epoch.
    */
    enum PackedDateTime : uint64_t;

    /**
     * @brief Returns the PackedDateTime value for the given
     *      UTC date and time.
     */
    PackedDateTime pack(const DateTime& dateTime);

    DateTime unpack(PackedDateTime dateTime);

    DateTimeDelta getDateTimeDelta(PackedDateTime from, PackedDateTime to);

    PackedDateTime add(PackedDateTime from, DateTimeDelta delta);
}
