#pragma once

#include "transform.hpp"

namespace analog
{
    class frequency_shift : public transform
    {
    public:
        explicit frequency_shift(double delta_frequency);

        analog::function apply(const analog::function &input) const override;

    private:
        double delta_frequency_;
    };
}
