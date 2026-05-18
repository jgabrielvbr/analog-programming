#include "function.hpp"

#include <algorithm>

namespace analog
{
    void function::add_wave(const wave &wave)
    {
        const auto insertion_point = std::upper_bound(
            waves_.begin(),
            waves_.end(),
            wave.frequency(),
            [](double frequency, const analog::wave &candidate)
            {
                return frequency < candidate.frequency();
            });

        waves_.insert(insertion_point, wave);
    }

    bool function::sub_wave(double frequency)
    {
        const auto wave = std::lower_bound(
            waves_.begin(),
            waves_.end(),
            frequency,
            [](const analog::wave &candidate, double frequency)
            {
                return candidate.frequency() < frequency;
            });

        if (wave == waves_.end() || wave->frequency() != frequency)
        {
            return false;
        }

        waves_.erase(wave);
        return true;
    }

    const wave *function::get_wave(double frequency) const
    {
        const auto wave = std::lower_bound(
            waves_.begin(),
            waves_.end(),
            frequency,
            [](const analog::wave &candidate, double frequency)
            {
                return candidate.frequency() < frequency;
            });

        if (wave == waves_.end() || wave->frequency() != frequency)
        {
            return nullptr;
        }

        return &(*wave);
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
