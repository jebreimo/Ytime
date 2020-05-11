//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-04-23.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ytime/DateTime.hpp"

#include <iomanip>
#include <ostream>
#include <vector>
#include "Ytime/LeapSeconds.hpp"
#include "InternalDateTimeMath.hpp"

namespace Ytime
{
    namespace
    {
        std::vector<std::string_view>
        splitString(std::string_view s, char delimiter, size_t maxSplit)
        {
            if (maxSplit == 0)
                return {s};

            std::vector<std::string_view> result;
            size_t pos = 0;
            while (true)
            {
                auto nextPos = s.find(delimiter, pos);
                result.push_back(s.substr(pos, nextPos - pos));
                if (nextPos == std::string_view::npos)
                    break;
                pos = nextPos + 1;
                if (result.size() == maxSplit)
                {
                    result.push_back(s.substr(pos));
                    break;
                }
            }
            return result;
        }

        std::optional<long> parseLong(const std::string& str, int base = 10)
        {
            char* endp = nullptr;
            errno = 0;
            auto value = strtol(str.c_str(), &endp, base);
            if (endp == str.c_str() + str.size() && errno == 0)
                return value;
            return {};
        }

        std::optional<long> parseFraction(const std::string& str, size_t digits)
        {
            auto n = parseLong(str);
            if (!n)
                return {};
            if (str.size() == digits)
                return n;

            auto power = std::abs(int64_t(digits) - int64_t(str.size()));
            long factor = 10;
            for (int i = 1; i < power; ++i)
                factor *= 10;
            if (str.size() < digits)
                return *n * factor;
            return *n / factor;
        }

        constexpr bool isLeapYear(uint32_t year) noexcept
        {
            return (year % 16 == 0) || (year % 4 == 0 && year % 25 != 0);
        }

        int getDaysInMonth(int year, int month)
        {
            constexpr int DAYS[12] = {
                31, 0, 31, 30, 31, 30,
                31, 31, 30, 31, 30, 31};
            if (month < 1 || 12 < month)
                return 0;
            if (month != 2)
                return DAYS[month - 1];
            return isLeapYear(year) ? 29 : 28;
        }

        std::string validateDate(const Date& date)
        {
            if (date.year < MIN_YEAR)
                return "Year must be at least " + std::to_string(MIN_YEAR);
            auto daysInMonth = getDaysInMonth(date.year, date.month);

            if (daysInMonth == 0)
                return "Month must be between 1 and 12.";
            if (date.day < 1 || daysInMonth < date.day)
                return "Day must be between 1 and "
                       + std::to_string(daysInMonth) + ".";
            return {};
        }

        std::string validateTime(const Date& date,
                                 const Time& time)
        {
            if (time.hour < 0 || 23 < time.hour)
                return "Hour must be between 0 and 23.";
            if (time.minute < 0 || 59 < time.minute)
                return "Minute must be between 0 and 59.";
            if (time.usecond < 0 || 1000000 <= time.usecond)
                return "Microsecond must be between 0 and 999999.";
            if (0 <= time.second && time.second <= 59)
                return {};

            if (time.hour != 23 || time.minute != 59)
                return "Second must be between 0 and 59.";

            auto maxSeconds = 59 + (hasLeapSecond(date) ? 1 : 0);
            if (time.second != maxSeconds)
                return "Second must be between 1 and "
                       + std::to_string(maxSeconds) + ".";
            return {};
        }
    }

    bool operator==(const Date& a, const Date& b)
    {
        return a.year == b.year && a.month == b.month && a.day == b.day;
    }

    bool operator!=(const Date& a, const Date& b)
    {
        return !(a == b);
    }

    bool operator<(const Date& a, const Date& b)
    {
        if (a.year != b.year)
            return a.year < b.year;
        if (a.month != b.month)
            return a.month < b.month;
        return a.day < b.day;
    }

    bool operator>(const Date& a, const Date& b)
    {
        return b < a;
    }

    bool operator<=(const Date& a, const Date& b)
    {
        return !(b < a);
    }

    bool operator>=(const Date& a, const Date& b)
    {
        return !(a < b);
    }

    std::ostream& operator<<(std::ostream& os, const Date& ymd)
    {
        auto prevFlags = os.setf(std::ios::dec, std::ios::basefield);
        auto prevFill = os.fill('0');
        os << ymd.year
           << '-' << std::setw(2) << ymd.month
           << '-' << std::setw(2) << ymd.day;
        os.fill(prevFill);
        os.flags(prevFlags);
        return os;
    }

    bool operator==(const Time& a, const Time& b)
    {
        return a.hour == b.hour
               && a.minute == b.minute
               && a.second == b.second
               && a.usecond == b.usecond;
    }

    bool operator!=(const Time& a, const Time& b)
    {
        return !(a == b);
    }

    bool operator<(const Time& a, const Time& b)
    {
        if (a.hour != b.hour)
            return a.hour < b.hour;
        if (a.minute != b.minute)
            return a.minute < b.minute;
        if (a.second != b.second)
            return a.second < b.second;
        return a.usecond < b.usecond;
    }

    bool operator>(const Time& a, const Time& b)
    {
        return b < a;
    }

    bool operator<=(const Time& a, const Time& b)
    {
        return !(b < a);
    }

    bool operator>=(const Time& a, const Time& b)
    {
        return !(a < b);
    }

    std::ostream& operator<<(std::ostream& os, const Time& hms)
    {
        auto prevFlags = os.setf(std::ios::dec, std::ios::basefield);
        auto prevFill = os.fill('0');
        os << std::setw(2) << hms.hour
           << ':' << std::setw(2) << hms.minute
           << ':' << std::setw(2) << hms.second
           << '.' << std::setw(6) << hms.usecond;
        os.fill(prevFill);
        os.flags(prevFlags);
        return os;
    }

    bool operator==(const DateTime& a, const DateTime& b)
    {
        return a.date == b.date && a.time == b.time;
    }

    bool operator!=(const DateTime& a, const DateTime& b)
    {
        return !(a == b);
    }

    bool operator<(const DateTime& a, const DateTime& b)
    {
        if (a.date != b.date)
            return a.date < b.date;
        return a.time < b.time;
    }

    bool operator>(const DateTime& a, const DateTime& b)
    {
        return b < a;
    }

    bool operator<=(const DateTime& a, const DateTime& b)
    {
        return !(b < a);
    }

    bool operator>=(const DateTime& a, const DateTime& b)
    {
        return !(a < b);
    }

    std::ostream& operator<<(std::ostream& os, const DateTime& dt)
    {
        return os << dt.date << "T" << dt.time;
    }

    std::optional<Date> parseYMD(std::string_view str)
    {
        auto parts = splitString(str, '-', 2);
        if (parts.size() != 3)
            return {};
        auto y = parseLong(std::string(parts[0]));
        auto m = parseLong(std::string(parts[1]));
        auto d = parseLong(std::string(parts[2]));
        if (y && m && d)
            return Date(*y, *m, *d);
        return {};
    }

    std::optional<Time> parseHMS(std::string_view str)
    {
        auto parts1 = splitString(str, '.', 1);
        auto parts2 = splitString(parts1[0].substr(), ':', 2);
        if (parts2.size() != 3)
            return {};
        auto h = parseLong(std::string(parts2[0]));
        auto m = parseLong(std::string(parts2[1]));
        auto s = parseLong(std::string(parts2[2]));
        auto u = parts1.size() == 2
                 ? parseFraction(std::string(parts1[1]), 6)
                 : std::optional<long>(0);

        if (h && m && s && u)
            return Time(int(*h), int(*m), int(*s), int(*u));
        return {};
    }

    std::optional<DateTime> parseDateTime(std::string_view str)
    {
        auto t = str.find('T');
        if (t != std::string_view::npos)
        {
            auto ymd = parseYMD(str.substr(0, t));
            auto hms = parseHMS(str.substr(t + 1));
            if (ymd && hms)
                return DateTime(*ymd, *hms);
            return {};
        }
        else if (str.find('-') != std::string_view::npos)
        {
            auto ymd = parseYMD(str);
            if (ymd)
                return DateTime(*ymd, {});
            return {};
        }
        else
        {
            auto hms = parseHMS(str);
            if (hms)
                return DateTime({}, *hms);
            return {};
        }
    }

    std::string validate(const DateTime& dateTime)
    {
        if (auto error = validateDate(dateTime.date); !error.empty())
            return error;
        return validateTime(dateTime.date, dateTime.time);
    }

    DateTime getCurrentDateTime()
    {
        time_t t = time(nullptr);
        struct tm* gt = gmtime(&t);
        return {{gt->tm_year + 1900, gt->tm_mon + 1, gt->tm_mday},
                {gt->tm_hour, gt->tm_min, gt->tm_sec}};
    }

    DateYD toYearDay(const Date& date)
    {
        Date date0 = {date.year, 1, 1};
        auto day = daysSinceEpochYMD(date) - daysSinceEpochYMD(date0);
        return {date.year, int(day + 1)};
    }

    Date toYearMonthDay(const DateYD& date)
    {
        return toYMD(daysSinceEpochYMD({date.year, 1, 1}) + date.day - 1);
    }
}
