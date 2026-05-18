#include "frequency_scale.hpp"

namespace analog
{
    frequency_scale::frequency_scale(double scale) : scale_(scale)
    {
    }

    analog::function frequency_scale::apply(const analog::function& input) const
    {
        analog::function output;

        for (const auto& wave : input.waves())
        {
            output.add_wave(analog::wave{wave.frequency() * scale_, wave.amplitude(), wave.phase()});
        }

        return output;
    }
}
