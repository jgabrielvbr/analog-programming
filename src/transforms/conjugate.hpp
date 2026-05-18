#pragma once

#include "transform.hpp"

namespace analog
{
    class conjugate : public transform
    {
    public:
        analog::function apply(const analog::function &input) const override;
    };
}
