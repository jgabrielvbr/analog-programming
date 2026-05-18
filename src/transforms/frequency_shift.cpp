#include "frequency_shift.hpp"

namespace analog
{
    frequency_shift::frequency_shift(double delta_frequency) : delta_frequency_(delta_frequency)
    {
    }

    analog::function frequency_shift::apply(const analog::function& input) const
    {
        analog::function output;

        for (const auto& wave : input.waves())
        {
            output.add_wave(analog::wave{wave.frequency() + delta_frequency_, wave.amplitude(), wave.phase()});
        }

        return output;
    }
}
