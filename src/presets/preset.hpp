#pragma once

#include "../core/function.hpp"

namespace analog
{
    class preset
    {
    public:
        virtual ~preset() = default;
        virtual double evaluate(double frequency) const = 0;
        virtual bool select(double frequency) const;

        analog::function to_function(const analog::function& input) const;
    };
}
