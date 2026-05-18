#include "difference.hpp"

#include <cmath>

namespace analog
{
    double difference::measure(const function& left, const function& right, double start, double end, int samples) const
    {
        const double step = (end - start) / static_cast<double>(samples - 1);
        double mse = 0.0;

        for (int i = 0; i < samples; ++i)
        {
            const double x = start + step * static_cast<double>(i);
            const double delta = std::real(left.evaluate(x)) - std::real(right.evaluate(x));
            mse += delta * delta;
        }

        return mse / static_cast<double>(samples);
    }
}
