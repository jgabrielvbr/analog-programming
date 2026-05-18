#include "modulation.hpp"
#include "product.hpp"

namespace analog
{
    function modulation::apply(const function& carrier, const function& modulator) const
    {
        const product prod;
        return prod.apply(carrier, modulator);
    }
}
