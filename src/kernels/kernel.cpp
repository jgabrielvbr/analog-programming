#include "kernel.hpp"

#include "../core/interaction.hpp"

#include <complex>
#include <cmath>
#include <utility>

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

    kernel::kernel(int label, function response) : label_(label), response_(std::move(response))
    {
    }

    int kernel::label() const
    {
        return label_;
    }

    const function& kernel::response() const
    {
        return response_;
    }

    double kernel::correlate(const function& input, double start, double end, int samples) const
    {
        return interaction::correlation(input, response_, start, end, samples);
    }

    double kernel::difference(const function& input, double start, double end, int samples) const
    {
        return interaction::difference(input, response_, start, end, samples);
    }

    double kernel::spectral_correlate(const function& input) const
    {
        double score = 0.0;

        const auto& left = input.waves();
        const auto& right = response_.waves();

        auto left_wave = left.begin();
        auto right_wave = right.begin();

        while (left_wave != left.end() && right_wave != right.end())
        {
            if (left_wave->frequency() < right_wave->frequency())
            {
                ++left_wave;
            }
            else if (right_wave->frequency() < left_wave->frequency())
            {
                ++right_wave;
            }
            else
            {
                score += std::real(std::conj(coefficient(*left_wave)) * coefficient(*right_wave));
                ++left_wave;
                ++right_wave;
            }
        }

        return score;
    }

    double kernel::spectral_difference(const function& input) const
    {
        double score = 0.0;

        const auto& left = input.waves();
        const auto& right = response_.waves();

        auto left_wave = left.begin();
        auto right_wave = right.begin();

        while (left_wave != left.end() || right_wave != right.end())
        {
            if (right_wave == right.end() || (left_wave != left.end() && left_wave->frequency() < right_wave->frequency()))
            {
                score += std::norm(coefficient(*left_wave));
                ++left_wave;
            }
            else if (left_wave == left.end() || right_wave->frequency() < left_wave->frequency())
            {
                score += std::norm(coefficient(*right_wave));
                ++right_wave;
            }
            else
            {
                score += std::norm(coefficient(*left_wave) - coefficient(*right_wave));
                ++left_wave;
                ++right_wave;
            }
        }

        return score;
    }
}
