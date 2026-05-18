#include "conjugate.hpp"

namespace analog
{
    analog::function conjugate::apply(const analog::function& input) const
    {
        analog::function output;

        for (const auto& wave : input.waves())
        {
            output.add_wave(analog::wave{-wave.frequency(), wave.amplitude(), -wave.phase()});
        }

        return output;
    }
}
