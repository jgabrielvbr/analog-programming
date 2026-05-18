#pragma once

#include "interaction.hpp"

namespace analog
{
    // Modulation is frequency mixing via pointwise multiplication in the domain.
    // Mathematically identical to product: every pair of waves (a, b) produces
    // a wave at frequency a+b with amplitude a*b and phase a+b.
    // This class delegates to product and exists as a named concept.
    class modulation : public interaction
    {
    public:
        function apply(const function& carrier, const function& modulator) const override;
    };
}
