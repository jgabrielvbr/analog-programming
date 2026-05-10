#include "function.hpp"

namespace analog
{
    void function::add_wave(const wave &wave)
    {
        waves_.push_back(wave);
    }

    std::complex<double> function::evaluate(double x) const
    {
        std::complex<double> result{0.0, 0.0};

        for (const auto &wave : waves_)
        {
            result += wave.evaluate(x);
        }

        return result;
    }

    double function::evaluate_real(double x) const
    {
        return evaluate(x).real();
    }

    double function::evaluate_imag(double x) const
    {
        return evaluate(x).imag();
    }

    const std::vector<wave> &function::waves() const
    {
        return waves_;
    }
}
