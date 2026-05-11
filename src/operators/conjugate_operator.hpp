#pragma once

#include "operator.hpp"

namespace analog
{
    class conjugate_operator : public signal_operator
    {
        public:
            analog::function apply(const analog::function& input) const override;
    };
}
