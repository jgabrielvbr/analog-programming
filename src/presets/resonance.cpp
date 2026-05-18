#include "resonance.hpp"

#include <cassert>
#include <cmath>

namespace analog
{
    resonance::resonance(double center_frequency, double bandwidth) : center_frequency_(center_frequency), bandwidth_(bandwidth)
    {
        assert(bandwidth > 0.0);
    }

    double resonance::evaluate(double frequency) const
    {
        const double deviation = (std::abs(frequency) - center_frequency_) / bandwidth_;
        return 1.0 / (1.0 + deviation * deviation);
    }
}
