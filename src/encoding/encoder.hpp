#pragma once

#include "../core/function.hpp"

#include "sample.hpp"

namespace analog
{
    class encoder
    {
        public:
            virtual ~encoder() = default;

            virtual function encode(const sample &input) const = 0;
    };
}