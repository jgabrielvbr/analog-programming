#include "band_pass.hpp"

#include <cmath>

namespace analog
{
    band_pass::band_pass(double min_frequency, double max_frequency) : min_frequency_(min_frequency), max_frequency_(max_frequency) {}

    double band_pass::evaluate(double frequency) const
    {
        const double f = std::abs(frequency);
        return (f >= min_frequency_ && f <= max_frequency_) ? 1.0 : 0.0;
    }

    bool band_pass::select(double frequency) const
    {
        const double f = std::abs(frequency);
        return f >= min_frequency_ && f <= max_frequency_;
    }
}
