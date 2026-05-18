#include "high_pass.hpp"

#include <cmath>

namespace analog
{
    high_pass::high_pass(double cutoff_frequency) : cutoff_frequency_(cutoff_frequency) {}

    double high_pass::evaluate(double frequency) const
    {
        return std::abs(frequency) >= cutoff_frequency_ ? 1.0 : 0.0;
    }

    bool high_pass::select(double frequency) const
    {
        return std::abs(frequency) >= cutoff_frequency_;
    }
}
