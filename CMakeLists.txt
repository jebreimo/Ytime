# ===========================================================================
# Copyright © 2020 Jan Erik Breimo. All rights reserved.
# Created by Jan Erik Breimo on 2020-04-23.
#
# This file is distributed under the BSD License.
# License text is included with the source distribution.
# ===========================================================================
cmake_minimum_required(VERSION 3.15)
project(utc2gnss VERSION 0.1.1)

set(CMAKE_CXX_STANDARD 17)

add_library(Ytime STATIC
    include/Ytime/Constants.hpp
    include/Ytime/DateTimeDelta.cpp
    include/Ytime/DateTimeDelta.hpp
    include/Ytime/DateTime.hpp
    include/Ytime/LeapSeconds.hpp
    include/Ytime/PackedDateTime.hpp
    include/Ytime/YtimeException.hpp
    src/Ytime/DateTime.cpp
    src/Ytime/InternalDateTimeMath.cpp
    src/Ytime/InternalDateTimeMath.hpp
    src/Ytime/LeapSeconds.cpp
    src/Ytime/PackedDateTime.cpp
    src/Ytime/YtimeThrow.hpp
    )

target_include_directories(Ytime
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    )

add_library(Ytime::Ytime ALIAS Ytime)

enable_testing(TRUE)

add_subdirectory(tests/YtimeTest)
