#include "band_stop.hpp"

#include <cmath>

namespace analog
{
    band_stop::band_stop(double min_frequency, double max_frequency) : min_frequency_(min_frequency), max_frequency_(max_frequency) {}

    double band_stop::evaluate(double frequency) const
    {
        const double f = std::abs(frequency);
        return (f >= min_frequency_ && f <= max_frequency_) ? 0.0 : 1.0;
    }

    bool band_stop::select(double frequency) const
    {
        const double f = std::abs(frequency);
        return !(f >= min_frequency_ && f <= max_frequency_);
    }
}
