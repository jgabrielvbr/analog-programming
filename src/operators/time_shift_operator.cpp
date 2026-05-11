#include "time_shift_operator.hpp"

namespace analog
{
    time_shift_operator::time_shift_operator(double shift) : shift_(shift)
    {
    }

    analog::function time_shift_operator::apply(const analog::function& input) const
    {
        analog::function output;

        for (const auto& wave : input.waves())
        {
            output.add_wave(analog::wave{wave.frequency(), wave.amplitude(), wave.phase() - wave.frequency() * shift_});
        }

        return output;
    }
}
