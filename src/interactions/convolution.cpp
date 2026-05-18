#include "convolution.hpp"

#include <cmath>

namespace analog
{
    function convolution::apply(const function& left, const function& right) const
    {
        function result;

        for (const auto& a : left.waves())
        {
            for (const auto& b : right.waves())
            {
                if (a.frequency() == b.frequency())
                {
                    result.add_wave(wave{a.frequency(), a.amplitude() * b.amplitude(), a.phase() + b.phase()});
                    break;
                }
            }
        }

        return result;
    }

    std::vector<double> convolution::apply_samples(const function& left, const function& right, double start, double end, int samples) const
    {
        const double step = (end - start) / static_cast<double>(samples - 1);

        std::vector<double> left_samples(samples);
        std::vector<double> right_samples(samples);

        for (int i = 0; i < samples; ++i)
        {
            const double x = start + step * static_cast<double>(i);
            left_samples[i]  = std::real(left.evaluate(x));
            right_samples[i] = std::real(right.evaluate(x));
        }

        std::vector<double> result(samples, 0.0);

        for (int i = 0; i < samples; ++i)
        {
            for (int j = 0; j <= i; ++j)
                result[i] += left_samples[j] * right_samples[i - j];

            result[i] *= step;
        }

        return result;
    }
}
