#include "product.hpp"

namespace analog
{
    function product::apply(const function &left, const function &right) const
    {
        function result;

        for (const auto &a : left.waves())
        {
            for (const auto &b : right.waves())
            {
                result.add_wave(analog::wave(a.frequency() + b.frequency(), a.amplitude() * b.amplitude(), a.phase() + b.phase()));
            }
        }

        return result;
    }
}