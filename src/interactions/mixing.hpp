#pragma once

#include "interaction.hpp"

namespace analog
{
    // Produces both the sum (upper) and difference (lower) sideband for every
    // pair of waves, each at half the product amplitude.
    class mixing : public interaction
    {
    public:
        function apply(const function& left, const function& right) const override;
    };
}
