#include "wave.hpp"

#include <complex>

namespace analog
{
    wave::wave(double frequency, double amplitude) : wave(frequency, amplitude, 0.0)
    {
    }

    wave::wave(double frequency, double amplitude, double phase) : frequency_(frequency), amplitude_(amplitude), phase_(phase)
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

    double wave::phase() const
    {
        return phase_;
    }

    std::complex<double> wave::evaluate(double x) const
    {
        const std::complex<double> j{0.0, 1.0};
        return amplitude_ * std::exp(j * (frequency_ * x + phase_));
    }
}
