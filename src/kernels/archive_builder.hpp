#pragma once

#include "archive.hpp"

#include <complex>
#include <cstddef>
#include <map>

namespace analog
{
    class archive_builder
    {
    public:
        void observe(int label, const function& response);

        archive build() const;

    private:
        struct accumulator
        {
            std::size_t count = 0;
            std::map<double, std::complex<double>> coefficients;
        };

        std::map<int, accumulator> accumulators_;
    };
}
