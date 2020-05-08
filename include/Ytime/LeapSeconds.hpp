//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-04-23.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "PackedDateTime.hpp"

namespace Ytime
{
    uint32_t getLeapSeconds(PackedDateTime dateTime) noexcept;

    bool isLeapSecond(PackedDateTime dateTime) noexcept;

    uint32_t getLeapSeconds(DateTime dateTime) noexcept;

    bool isLeapSecond(DateTime dateTime) noexcept;

    uint32_t getLeapSeconds(YearMonthDay date) noexcept;

    bool hasLeapSecond(YearMonthDay date) noexcept;
}
