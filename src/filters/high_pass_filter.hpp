#pragma once

#include "filter.hpp"

namespace analog
{
    class high_pass_filter : public filter
    {
        public:
            explicit high_pass_filter(double cutoff_frequency);

            analog::function apply(const analog::function& input) const override;

        private:
            double cutoff_frequency_;
    };
}