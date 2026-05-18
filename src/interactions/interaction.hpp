#pragma once

#include "../core/function.hpp"

namespace analog
{
    class interaction
    {
        public:
            virtual ~interaction() = default;

            virtual function apply(const function &left, const function &right) const = 0;
};
}