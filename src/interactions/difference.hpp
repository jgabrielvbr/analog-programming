#pragma once

#include "../core/function.hpp"

namespace analog
{
    class difference
    {
    public:
        double measure(const function& left, const function& right, double start, double end, int samples) const;
    };
}
