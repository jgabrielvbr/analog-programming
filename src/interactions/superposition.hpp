#pragma once

#include "interaction.hpp"

namespace analog
{
    class superposition : public interaction
    {
        public:
            function apply(const function& left, const function& right) const override;
    };
}