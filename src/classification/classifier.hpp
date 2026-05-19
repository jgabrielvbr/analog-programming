#pragma once

#include "prototype.hpp"

#include "../core/function.hpp"

#include <vector>

namespace analog
{
    class classifier
    {
    public:
        void add_prototype(const prototype &prototype);

        int classify(const function &input) const;

    private:
        std::vector<prototype> prototypes_;
    };
}