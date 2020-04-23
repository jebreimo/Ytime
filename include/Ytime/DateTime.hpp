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
    struct YearDay
    {
        int year, day;

        constexpr YearDay() noexcept
            : year(), day()
        {}

        constexpr YearDay(int year, int day) noexcept
            : year(year), day(day)
        {}
    };

    struct YearMonthDay
    {
        int year, month, day;

        constexpr YearMonthDay() noexcept
            : year(), month(), day()
        {}

        constexpr YearMonthDay(int year, int month, int day) noexcept
            : year(year), month(month), day(day)
        {}
    };

    bool operator==(const YearMonthDay& a, const YearMonthDay& b);

    bool operator!=(const YearMonthDay& a, const YearMonthDay& b);

    bool operator<(const YearMonthDay& a, const YearMonthDay& b);

    bool operator>(const YearMonthDay& a, const YearMonthDay& b);

    bool operator<=(const YearMonthDay& a, const YearMonthDay& b);

    bool operator>=(const YearMonthDay& a, const YearMonthDay& b);

    std::ostream& operator<<(std::ostream& os, const YearMonthDay& ymd);

    struct HourMinuteSecond
    {
        int hour, minute, second, usecond;

        constexpr HourMinuteSecond() noexcept
            : hour(0), minute(0), second(0), usecond(0)
        {}

        constexpr HourMinuteSecond(int hour, int minute, int second, int usecond = 0) noexcept
            : hour(hour), minute(minute), second(second), usecond(usecond)
        {}
    };

    bool operator==(const HourMinuteSecond& a, const HourMinuteSecond& b);

    bool operator!=(const HourMinuteSecond& a, const HourMinuteSecond& b);

    bool operator<(const HourMinuteSecond& a, const HourMinuteSecond& b);

    bool operator>(const HourMinuteSecond& a, const HourMinuteSecond& b);

    bool operator<=(const HourMinuteSecond& a, const HourMinuteSecond& b);

    bool operator>=(const HourMinuteSecond& a, const HourMinuteSecond& b);

    std::ostream& operator<<(std::ostream& os, const HourMinuteSecond& hms);

    struct DateTime
    {
        YearMonthDay date;
        HourMinuteSecond time;

        constexpr DateTime() noexcept
            : date(), time()
        {}

        constexpr DateTime(YearMonthDay date, HourMinuteSecond time) noexcept
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

    enum Days : int64_t;
    enum Seconds : int64_t;
    enum Useconds : int64_t;

    class Delta
    {
    public:
        constexpr Delta() noexcept
            : m_Days(), m_Useconds()
        {}

        constexpr Delta(Days days, Useconds totalUsecs) noexcept
            : m_Days(days), m_Useconds(totalUsecs)
        {}

        constexpr Delta(Days days, Seconds secs) noexcept
            : m_Days(days), m_Useconds(Useconds(secs * USECS_PER_SEC))
        {}

        constexpr Delta(Days days, Seconds secs, Useconds usecs) noexcept
            : m_Days(days),
              m_Useconds(Useconds(secs * USECS_PER_SEC + usecs))
        {}

        constexpr Days days() const noexcept
        {
            return Days(m_Days);
        }

        constexpr Seconds seconds() const noexcept
        {
            return Seconds(m_Useconds / int64_t(USECS_PER_SEC));
        }

        constexpr Useconds useconds() const noexcept
        {
            return Useconds(m_Useconds - seconds() * int64_t(USECS_PER_SEC));
        }

        constexpr Useconds totalUseconds() const noexcept
        {
            return Useconds(m_Useconds);
        }

    private:
        Days m_Days;
        Useconds m_Useconds;
    };

    bool operator==(const Delta& a, const Delta& b) noexcept;

    bool operator!=(const Delta& a, const Delta& b) noexcept;

    std::ostream& operator<<(std::ostream& os, const Delta& d);

    std::optional<YearMonthDay> parseYMD(std::string_view str);

    std::optional<HourMinuteSecond> parseHMS(std::string_view str);

    std::optional<DateTime> parseDateTime(std::string_view str);

    std::string validate(const DateTime& dateTime);

    DateTime getCurrentDateTime();

    YearDay toYearDay(const YearMonthDay& date);

    YearMonthDay toYearMonthDay(const YearDay& date);
}
