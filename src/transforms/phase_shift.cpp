#include "phase_shift.hpp"

namespace analog
{
    phase_shift::phase_shift(double phase) : phase_(phase)
    {
    }

    analog::function phase_shift::apply(const analog::function& input) const
    {
        analog::function output;

        for (const auto& wave : input.waves())
        {
            output.add_wave(analog::wave{wave.frequency(), wave.amplitude(), wave.phase() + phase_});
        }

        return output;
    }
}
