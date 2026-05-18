#pragma once

#include "interaction.hpp"
#include <vector>

namespace analog
{
    class convolution : public interaction
    {
    public:
        // Spectral convolution: frequency-matched product (used for preset filtering)
        function apply(const function& left, const function& right) const override;

        // Numerical convolution: discrete approximation over a sampled interval
        std::vector<double> apply_samples(const function& left, const function& right, double start, double end, int samples) const;
    };
}
