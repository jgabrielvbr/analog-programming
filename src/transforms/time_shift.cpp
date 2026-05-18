#include "time_shift.hpp"

namespace analog
{
    time_shift::time_shift(double shift) : shift_(shift)
    {
    }

    analog::function time_shift::apply(const analog::function& input) const
    {
        analog::function output;

        for (const auto& wave : input.waves())
        {
            output.add_wave(analog::wave{wave.frequency(), wave.amplitude(), wave.phase() - wave.frequency() * shift_});
        }

        return output;
    }
}
