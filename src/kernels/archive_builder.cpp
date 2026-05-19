#include "archive_builder.hpp"

#include <cmath>

namespace analog
{
    namespace
    {
        std::complex<double> coefficient(const wave& wave)
        {
            const std::complex<double> j{0.0, 1.0};
            return wave.amplitude() * std::exp(j * wave.phase());
        }
    }

    void archive_builder::observe(int label, const function& response)
    {
        auto& accumulator = accumulators_[label];
        ++accumulator.count;

        for (const auto& wave : response.waves())
        {
            accumulator.coefficients[wave.frequency()] += coefficient(wave);
        }
    }

    archive archive_builder::build() const
    {
        archive result;

        for (const auto& [label, accumulator] : accumulators_)
        {
            if (accumulator.count == 0)
            {
                continue;
            }

            function response;

            for (const auto& [frequency, sum] : accumulator.coefficients)
            {
                const auto mean = sum / static_cast<double>(accumulator.count);
                const double amplitude = std::abs(mean);

                if (amplitude > 0.0)
                {
                    response.add_wave({frequency, amplitude, std::arg(mean)});
                }
            }

            result.add_kernel({label, response});
        }

        return result;
    }
}
