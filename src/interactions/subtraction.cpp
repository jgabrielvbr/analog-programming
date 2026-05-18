#include "subtraction.hpp"

namespace analog
{
    function subtraction::apply(const function &left, const function &right) const
    {
        function result;

        for (const auto &wave : left.waves())
        {
            result.add_wave(wave);
        }

        for (const auto &wave : right.waves())
        {
            result.add_wave(analog::wave(wave.frequency(), -wave.amplitude(), wave.phase()));
        }

        return result;
    }
}