#pragma once

#include "../core/function.hpp"

namespace analog
{
    class kernel
    {
    public:
        // A kernel is a labeled response function.
        kernel(int label, function response);

        int label() const;
        const function& response() const;

        double correlate(const function& input, double start, double end, int samples) const;
        double difference(const function& input, double start, double end, int samples) const;
        double spectral_correlate(const function& input) const;
        double spectral_difference(const function& input) const;

    private:
        int label_;
        function response_;
    };
}
