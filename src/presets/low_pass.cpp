#include "low_pass.hpp"

#include <cmath>

namespace analog
{
    low_pass::low_pass(double cutoff_frequency) : cutoff_frequency_(cutoff_frequency) {}

    double low_pass::evaluate(double frequency) const
    {
        return std::abs(frequency) <= cutoff_frequency_ ? 1.0 : 0.0;
    }

    bool low_pass::select(double frequency) const
    {
        return std::abs(frequency) <= cutoff_frequency_;
    }
}
