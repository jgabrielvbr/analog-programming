#pragma once

#include "filter.hpp"

namespace analog
{
    class band_stop_filter : public filter
    {
        public:
            band_stop_filter(double min_frequency, double max_frequency);

            analog::function apply(const analog::function& input) const override;

        private:
            double min_frequency_;
            double max_frequency_;
    };
}