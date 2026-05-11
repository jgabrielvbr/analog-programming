#pragma once

#include "operator.hpp"

namespace analog
{
    class frequency_scale_operator : public signal_operator
    {
        public:
            explicit frequency_scale_operator(double scale);

            analog::function apply(const analog::function& input) const override;

        private:
            double scale_;
    };
}
