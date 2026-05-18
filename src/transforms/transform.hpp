#pragma once

#include "../core/function.hpp"

namespace analog
{
    class transform
    {
        public:
            virtual ~transform() = default;

            virtual analog::function apply(const analog::function &input) const = 0;
    };
}