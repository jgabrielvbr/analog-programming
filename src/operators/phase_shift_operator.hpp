#pragma once

#include "operator.hpp"

namespace analog
{
    class phase_shift_operator : public signal_operator
    {
        public:
            explicit phase_shift_operator(double phase);

            analog::function apply(const analog::function& input) const override;

        private:
            double phase_;
    };
}
