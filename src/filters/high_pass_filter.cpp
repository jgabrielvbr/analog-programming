#include "high_pass_filter.hpp"

#include <cmath>

namespace analog
{
    high_pass_filter::high_pass_filter(double cutoff_frequency) : cutoff_frequency_(cutoff_frequency)
    {
    }

    analog::function high_pass_filter::apply(const analog::function &input) const
    {
        analog::function output;

        for (const auto &wave : input.waves())
        {
            if (std::abs(wave.frequency()) >= cutoff_frequency_)
            {
                output.add_wave(wave);
            }
        }

        return output;
    }
}