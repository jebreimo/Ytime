//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-04-23.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Ytime/YtimeException.hpp"

#define _YTIME_THROW_3(file, line, msg) \
    throw ::Ytime::YtimeException(file ":" #line ": " msg)

#define _YTIME_THROW_2(file, line, msg) \
    _YTIME_THROW_3(file, line, msg)

#define YTIME_THROW(msg) \
    _YTIME_THROW_2(__FILE__, __LINE__, msg)
