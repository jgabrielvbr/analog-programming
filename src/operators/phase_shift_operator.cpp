#include "phase_shift_operator.hpp"

namespace analog
{
    phase_shift_operator::phase_shift_operator(double phase) : phase_(phase)
    {
    }

    analog::function phase_shift_operator::apply(const analog::function& input) const
    {
        analog::function output;

        for (const auto& wave : input.waves())
        {
            output.add_wave(analog::wave{wave.frequency(), wave.amplitude(), wave.phase() + phase_});
        }

        return output;
    }
}
