#include "mixing.hpp"

namespace analog
{
    function mixing::apply(const function& left, const function& right) const
    {
        function result;

        for (const auto& a : left.waves())
        {
            for (const auto& b : right.waves())
            {
                const double amplitude = 0.5 * a.amplitude() * b.amplitude();

                result.add_wave(wave{a.frequency() + b.frequency(), amplitude, a.phase() + b.phase()});
                result.add_wave(wave{a.frequency() - b.frequency(), amplitude, a.phase() - b.phase()});
            }
        }

        return result;
    }
}
