//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-04-23.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <iosfwd>
#include <optional>
#include <string>
#include "Constants.hpp"

namespace Ytime
{
    struct DateYD
    {
        int year, day;

        constexpr DateYD() noexcept
            : year(), day()
        {}

        constexpr DateYD(int year, int day) noexcept
            : year(year), day(day)
        {}
    };

    struct Date
    {
        int year, month, day;

        constexpr Date() noexcept
            : year(), month(), day()
        {}

        constexpr Date(int year, int month, int day) noexcept
            : year(year), month(month), day(day)
        {}
    };

    bool operator==(const Date& a, const Date& b);

    bool operator!=(const Date& a, const Date& b);

    bool operator<(const Date& a, const Date& b);

    bool operator>(const Date& a, const Date& b);

    bool operator<=(const Date& a, const Date& b);

    bool operator>=(const Date& a, const Date& b);

    std::ostream& operator<<(std::ostream& os, const Date& ymd);

    struct Time
    {
        int hour, minute, second, usecond;

        constexpr Time() noexcept
            : hour(0), minute(0), second(0), usecond(0)
        {}

        constexpr Time(int hour, int minute,
                       int second, int usecond = 0) noexcept
            : hour(hour), minute(minute), second(second), usecond(usecond)
        {}
    };

    bool operator==(const Time& a, const Time& b);

    bool operator!=(const Time& a, const Time& b);

    bool operator<(const Time& a, const Time& b);

    bool operator>(const Time& a, const Time& b);

    bool operator<=(const Time& a, const Time& b);

    bool operator>=(const Time& a, const Time& b);

    std::ostream& operator<<(std::ostream& os, const Time& hms);

    struct DateTime
    {
        Date date;
        Time time;

        constexpr DateTime() noexcept
            : date(), time()
        {}

        constexpr DateTime(Date date, Time time) noexcept
            : date(date.year, date.month, date.day),
              time(time.hour, time.minute, time.second, time.usecond)
        {}
    };

    bool operator==(const DateTime& a, const DateTime& b);

    bool operator!=(const DateTime& a, const DateTime& b);

    bool operator<(const DateTime& a, const DateTime& b);

    bool operator>(const DateTime& a, const DateTime& b);

    bool operator<=(const DateTime& a, const DateTime& b);

    bool operator>=(const DateTime& a, const DateTime& b);

    std::ostream& operator<<(std::ostream& os, const DateTime& dt);

    std::optional<Date> parseYMD(std::string_view str);

    std::optional<Time> parseHMS(std::string_view str);

    std::optional<DateTime> parseDateTime(std::string_view str);

    std::string validate(const DateTime& dateTime);

    DateTime getCurrentDateTime();

    DateYD toYearDay(const Date& date);

    Date toYearMonthDay(const DateYD& date);
}
