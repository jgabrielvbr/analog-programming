#include "frequency_scale_operator.hpp"

namespace analog
{
    frequency_scale_operator::frequency_scale_operator(double scale) : scale_(scale)
    {
    }

    analog::function frequency_scale_operator::apply(const analog::function& input) const
    {
        analog::function output;

        for (const auto& wave : input.waves())
        {
            output.add_wave(analog::wave{wave.frequency() * scale_, wave.amplitude(), wave.phase()});
        }

        return output;
    }
}
