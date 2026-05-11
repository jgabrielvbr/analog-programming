#pragma once

#include "operator.hpp"

namespace analog
{
    class time_shift_operator : public signal_operator
    {
        public:
            explicit time_shift_operator(double shift);

            analog::function apply(const analog::function& input) const override;

        private:
            double shift_;
    };
}
