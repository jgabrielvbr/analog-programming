#pragma once

#include "transform.hpp"

namespace analog
{
    class frequency_scale : public transform
    {
    public:
        explicit frequency_scale(double scale);

        analog::function apply(const analog::function &input) const override;

    private:
        double scale_;
    };
}
