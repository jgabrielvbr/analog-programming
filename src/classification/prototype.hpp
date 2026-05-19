#pragma once

#include "../core/function.hpp"

namespace analog
{
    class prototype
    {
    public:
        prototype(int label, function representation);

        int label() const;

        const function& representation() const;

    private:
        int label_;

        function representation_;
    };
}