#include "conjugate_operator.hpp"

namespace analog
{
    analog::function conjugate_operator::apply(const analog::function& input) const
    {
        analog::function output;

        for (const auto& wave : input.waves())
        {
            output.add_wave(analog::wave{-wave.frequency(), wave.amplitude(), -wave.phase()});
        }

        return output;
    }
}
