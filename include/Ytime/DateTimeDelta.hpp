//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-04-24.
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
    class DateTimeDelta
    {
    public:
        constexpr DateTimeDelta() noexcept
            : m_Days(), m_Useconds()
        {}

        constexpr DateTimeDelta(int64_t days, int64_t totalUsecs) noexcept
            : m_Days(days), m_Useconds(totalUsecs)
        {}

        constexpr bool isEmpty() const noexcept
        {
            return m_Days == 0 && m_Useconds == 0;
        }

        constexpr int64_t days() const noexcept
        {
            return m_Days;
        }

        constexpr int64_t seconds() const noexcept
        {
            return m_Useconds / int64_t(USECS_PER_SEC);
        }

        constexpr int64_t useconds() const noexcept
        {
            return m_Useconds - seconds() * int64_t(USECS_PER_SEC);
        }

        constexpr int64_t totalUseconds() const noexcept
        {
            return m_Useconds;
        }

    private:
        int64_t m_Days;
        int64_t m_Useconds;
    };

    constexpr bool
    operator==(const DateTimeDelta& a, const DateTimeDelta& b) noexcept
    {
        return a.days() == b.days() && a.totalUseconds() == b.totalUseconds();
    }

    constexpr bool
    operator!=(const DateTimeDelta& a, const DateTimeDelta& b) noexcept
    {
        return !(a == b);
    }

    constexpr DateTimeDelta
    operator+(const DateTimeDelta& a, const DateTimeDelta& b) noexcept
    {
        return {a.days() + b.days(), a.totalUseconds() + b.totalUseconds()};
    }

    constexpr DateTimeDelta
    operator-(const DateTimeDelta& a, const DateTimeDelta& b) noexcept
    {
        return {a.days() - b.days(), a.totalUseconds() - b.totalUseconds()};
    }

    constexpr DateTimeDelta
    operator-(const DateTimeDelta& d) noexcept
    {
        return {-d.days(), -d.totalUseconds()};
    }

    std::ostream& operator<<(std::ostream& os, const DateTimeDelta& d);

    constexpr DateTimeDelta Days(int64_t days) noexcept
    {
        return DateTimeDelta(days, 0);
    }

    constexpr DateTimeDelta Seconds(int64_t secs) noexcept
    {
        return DateTimeDelta(0, secs * int64_t(USECS_PER_SEC));
    }

    constexpr DateTimeDelta Useconds(int64_t usecs) noexcept
    {
        return DateTimeDelta(0, usecs);
    }
}
