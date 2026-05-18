#pragma once

#include "transform.hpp"

namespace analog
{
    class amplitude_scale : public transform
    {
    public:
        explicit amplitude_scale(double scale);

        analog::function apply(const analog::function &input) const override;

    private:
        double scale_;
    };
}
