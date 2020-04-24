//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-04-24.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "DateTimeDelta.hpp"

#include <iomanip>
#include <ostream>

namespace Ytime
{
    std::ostream& operator<<(std::ostream& os, const DateTimeDelta& d)
    {
        auto prevFill = os.fill('0');
        os << d.days() << " days ";
        auto secs = d.seconds();
        auto usecs = d.useconds();
        if (usecs != 0)
            os << secs << '.' << std::setw(6) << usecs << " seconds";
        else
            os << secs << " seconds";
        os.fill(prevFill);
        return os;
    }
}
