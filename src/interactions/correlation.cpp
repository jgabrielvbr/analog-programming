#include "correlation.hpp"
#include <complex>

namespace analog
{
    double correlation::measure(const function &left, const function &right, double start, double end, int samples) const
    {
        double result = 0.0;
        const double step = (end - start) / static_cast<double>(samples - 1);

        for (int i = 0; i < samples; ++i)
        {
            const double x = start + step * static_cast<double>(i);

            result += std::real(std::conj(left.evaluate(x)) * right.evaluate(x));
        }

        return result * step;
    }
}