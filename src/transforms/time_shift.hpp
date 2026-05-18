#pragma once

#include "transform.hpp"

namespace analog
{
    class time_shift : public transform
    {
    public:
        explicit time_shift(double shift);

        analog::function apply(const analog::function &input) const override;

    private:
        double shift_;
    };
}
