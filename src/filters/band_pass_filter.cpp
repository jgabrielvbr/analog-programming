#include "band_pass_filter.hpp"

#include <cmath>

namespace analog
{
    band_pass_filter::band_pass_filter(double min_frequency, double max_frequency) : min_frequency_(min_frequency), max_frequency_(max_frequency)
    {
    }

    analog::function band_pass_filter::apply(const analog::function &input) const
    {
        analog::function output;

        for (const auto &wave : input.waves())
        {
            const double frequency = std::abs(wave.frequency());

            if (min_frequency_ <= frequency && frequency <= max_frequency_)
            {
                output.add_wave(wave);
            }
        }

        return output;
    }
}