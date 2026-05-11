#pragma once

#include "../core/function.hpp"

namespace analog
{
    class signal_operator
    {
    public:
        virtual ~signal_operator() = default;

        virtual analog::function apply(const analog::function& input) const = 0;
    };
}
