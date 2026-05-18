#include "gaussian.hpp"

#include <cassert>
#include <cmath>

namespace analog
{
    gaussian::gaussian(double sigma) : sigma_(sigma)
    {
        assert(sigma > 0.0);
    }

    double gaussian::evaluate(double frequency) const
    {
        return std::exp(-(frequency * frequency) / (2.0 * sigma_ * sigma_));
    }
}
