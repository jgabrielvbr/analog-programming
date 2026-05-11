#pragma once

#include "operator.hpp"

namespace analog
{
    class frequency_shift_operator : public signal_operator
    {
        public:
            explicit frequency_shift_operator(double delta_frequency);

            analog::function apply(const analog::function& input) const override;

        private:
            double delta_frequency_;
    };
}
