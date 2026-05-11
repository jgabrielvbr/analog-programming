#pragma once

#include "operator.hpp"

namespace analog
{
    class amplitude_scale_operator : public signal_operator
    {
        public:
            explicit amplitude_scale_operator(double scale);

            analog::function apply(const analog::function& input) const override;

        private:
            double scale_;
    };
}
