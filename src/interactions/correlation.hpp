#pragma once

#include "interaction.hpp"

namespace analog
{
    class correlation
    {
        public:
            double measure(const function &left, const function &right, double start, double end, int samples) const;
    };
}