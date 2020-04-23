//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-04-23.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <stdexcept>
#include <string>

/**
 * @file
 * @brief Defines the YtimeException class.
 */

/**
 * @brief The namespace for all Argos classes and functions.
 */
namespace Ytime
{
    /**
     * @brief The exception class used throughout Argos.
     */
    class YtimeException : public std::runtime_error
    {
    public:
        YtimeException() noexcept
            : std::runtime_error("Unspecified error.")
        {}

        explicit YtimeException(const std::string& message) noexcept
            : std::runtime_error(message)
        {}

        explicit YtimeException(const char* message) noexcept
            : std::runtime_error(message)
        {}
    };
}
