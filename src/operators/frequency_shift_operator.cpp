#include "frequency_shift_operator.hpp"

namespace analog
{
    frequency_shift_operator::frequency_shift_operator(double delta_frequency) : delta_frequency_(delta_frequency)
    {
    }

    analog::function frequency_shift_operator::apply(const analog::function& input) const
    {
        analog::function output;

        for (const auto& wave : input.waves())
        {
            output.add_wave(analog::wave{wave.frequency() + delta_frequency_, wave.amplitude(), wave.phase()});
        }

        return output;
    }
}
