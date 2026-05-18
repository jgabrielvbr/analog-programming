#include "amplitude_scale.hpp"

namespace analog
{
    amplitude_scale::amplitude_scale(double scale) : scale_(scale)
    {
    }

    analog::function amplitude_scale::apply(const analog::function& input) const
    {
        analog::function output;

        for (const auto& wave : input.waves())
        {
            output.add_wave(analog::wave{wave.frequency(), wave.amplitude() * scale_, wave.phase()});
        }

        return output;
    }
}
