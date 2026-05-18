#include "superposition.hpp"

namespace analog
{
    function superposition::apply(const function& left, const function& right) const
    {
        function result;

        for (const auto& wave : left.waves())
        {
            result.add_wave(wave);
        }

        for (const auto& wave : right.waves())
        {
            result.add_wave(wave);
        }

        return result;
    }
}