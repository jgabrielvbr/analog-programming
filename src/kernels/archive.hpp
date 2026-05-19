#pragma once

#include "kernel.hpp"

#include <vector>

namespace analog
{
    class archive
    {
    public:
        void add_kernel(const kernel& kernel);

        const std::vector<kernel>& kernels() const;

        int classify_by_correlation(const function& input, double start, double end, int samples) const;
        int classify_by_difference(const function& input, double start, double end, int samples) const;
        int classify_by_spectral_correlation(const function& input) const;
        int classify_by_spectral_difference(const function& input) const;

    private:
        // An archive is a collection of possible response structures.
        std::vector<kernel> kernels_;
    };
}
