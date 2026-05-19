#include "archive.hpp"

#include <limits>

namespace analog
{
    void archive::add_kernel(const kernel& kernel)
    {
        kernels_.push_back(kernel);
    }

    const std::vector<kernel>& archive::kernels() const
    {
        return kernels_;
    }

    int archive::classify_by_correlation(const function& input, double start, double end, int samples) const
    {
        if (kernels_.empty())
        {
            return -1;
        }

        double best_score = -std::numeric_limits<double>::infinity();
        int best_label = -1;

        for (const auto& kernel : kernels_)
        {
            const double score = kernel.correlate(input, start, end, samples);

            if (score > best_score)
            {
                best_score = score;
                best_label = kernel.label();
            }
        }

        return best_label;
    }

    int archive::classify_by_difference(const function& input, double start, double end, int samples) const
    {
        if (kernels_.empty())
        {
            return -1;
        }

        double best_score = std::numeric_limits<double>::infinity();
        int best_label = -1;

        for (const auto& kernel : kernels_)
        {
            const double score = kernel.difference(input, start, end, samples);

            if (score < best_score)
            {
                best_score = score;
                best_label = kernel.label();
            }
        }

        return best_label;
    }

    int archive::classify_by_spectral_correlation(const function& input) const
    {
        if (kernels_.empty())
        {
            return -1;
        }

        double best_score = -std::numeric_limits<double>::infinity();
        int best_label = -1;

        for (const auto& kernel : kernels_)
        {
            const double score = kernel.spectral_correlate(input);

            if (score > best_score)
            {
                best_score = score;
                best_label = kernel.label();
            }
        }

        return best_label;
    }

    int archive::classify_by_spectral_difference(const function& input) const
    {
        if (kernels_.empty())
        {
            return -1;
        }

        double best_score = std::numeric_limits<double>::infinity();
        int best_label = -1;

        for (const auto& kernel : kernels_)
        {
            const double score = kernel.spectral_difference(input);

            if (score < best_score)
            {
                best_score = score;
                best_label = kernel.label();
            }
        }

        return best_label;
    }
}
