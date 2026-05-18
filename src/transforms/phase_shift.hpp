#pragma once

#include "transform.hpp"

namespace analog
{
    class phase_shift : public transform
    {
    public:
        explicit phase_shift(double phase);

        analog::function apply(const analog::function &input) const override;

    private:
        double phase_;
    };
}
