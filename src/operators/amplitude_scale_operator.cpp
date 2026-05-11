#include "amplitude_scale_operator.hpp"

namespace analog
{
    amplitude_scale_operator::amplitude_scale_operator(double scale) : scale_(scale)
    {
    }

    analog::function amplitude_scale_operator::apply(const analog::function& input) const
    {
        analog::function output;

        for (const auto& wave : input.waves())
        {
            output.add_wave(analog::wave{wave.frequency(), wave.amplitude() * scale_, wave.phase()});
        }

        return output;
    }
}
