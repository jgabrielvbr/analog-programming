#include "band_stop_filter.hpp"

#include <cmath>

namespace analog
{
    band_stop_filter::band_stop_filter(double min_frequency, double max_frequency)
        : min_frequency_(min_frequency), max_frequency_(max_frequency)
    {
    }

    analog::function band_stop_filter::apply(const analog::function& input) const
    {
        analog::function output;

        for (const auto& wave : input.waves())
        {
            const double frequency = std::abs(wave.frequency());

            if (frequency < min_frequency_ || max_frequency_ < frequency)
            {
                output.add_wave(wave);
            }
        }

        return output;
    }
}