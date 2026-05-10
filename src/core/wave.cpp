#include "wave.hpp"

#include <complex>

namespace analog
{
    wave::wave(double frequency, double amplitude) : frequency_(frequency), amplitude_(amplitude)
    {
    }

    double wave::frequency() const
    {
        return frequency_;
    }

    double wave::amplitude() const
    {
        return amplitude_;
    }

    std::complex<double> wave::evaluate(double x) const
    {
        const std::complex<double> j{0.0, 1.0};
        return amplitude_ * std::exp(j * frequency_ * x);
    }
}
