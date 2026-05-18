#pragma once

#include "interaction.hpp"

namespace analog
{
    class product : public interaction
    {
        public:
            function apply(const function &left, const function &right) const override;
    };
}