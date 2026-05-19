#include "interaction.hpp"

#include <complex>
#include <cmath>
#include <vector>

namespace analog
{
    function interaction::superposition(const function &left, const function &right)
    {
        function result;

        for (const auto &wave : left.waves())
        {
            result.add_wave(wave);
        }

        for (const auto &wave : right.waves())
        {
            result.add_wave(wave);
        }

        return result;
    }

    function interaction::subtraction(const function &left, const function &right)
    {
        function result;

        for (const auto &wave : left.waves())
        {
            result.add_wave(wave);
        }

        for (const auto &wave : right.waves())
        {
            result.add_wave({wave.frequency(), -wave.amplitude(), wave.phase()});
        }

        return result;
    }

    function interaction::product(const function &left, const function &right)
    {
        function result;

        for (const auto &a : left.waves())
        {
            for (const auto &b : right.waves())
            {
                result.add_wave({a.frequency() + b.frequency(), a.amplitude() * b.amplitude(), a.phase() + b.phase()});
            }
        }

        return result;
    }

    function interaction::modulation(const function &carrier, const function &modulator)
    {
        return product(carrier, modulator);
    }

    function interaction::mixing(const function &left, const function &right)
    {
        function result;

        for (const auto &a : left.waves())
        {
            for (const auto &b : right.waves())
            {
                const double amplitude = 0.5 * a.amplitude() * b.amplitude();

                result.add_wave({a.frequency() + b.frequency(), amplitude, a.phase() + b.phase()});
                result.add_wave({a.frequency() - b.frequency(), amplitude, a.phase() - b.phase()});
            }
        }

        return result;
    }

    double interaction::correlation(const function &left, const function &right, double start, double end, int samples)
    {
        if (samples <= 1)
        {
            return 0.0;
        }

        double result = 0.0;
        const double step = (end - start) / static_cast<double>(samples - 1);

        for (int i = 0; i < samples; ++i)
        {
            const double x = start + step * static_cast<double>(i);
            result += std::real(std::conj(left.evaluate(x)) * right.evaluate(x));
        }

        return result * step;
    }

    double interaction::difference(const function &left, const function &right, double start, double end, int samples)
    {
        if (samples <= 0)
        {
            return 0.0;
        }

        const double step = samples == 1 ? 0.0 : (end - start) / static_cast<double>(samples - 1);

        double mse = 0.0;

        for (int i = 0; i < samples; ++i)
        {
            const double x = start + step * static_cast<double>(i);
            const double delta = std::real(left.evaluate(x)) - std::real(right.evaluate(x));
            mse += delta * delta;
        }

        return mse / static_cast<double>(samples);
    }

    function interaction::convolution(const function &left, const function &right, double period, double epsilon)
    {
        function result;

        if (period <= 0.0)
        {
            return result;
        }

        for (const auto &a : left.waves())
        {
            for (const auto &b : right.waves())
            {
                if (std::abs(a.frequency() - b.frequency()) <= epsilon)
                {
                    result.add_wave({b.frequency(),
                                     period * a.amplitude() * b.amplitude(),
                                     a.phase() + b.phase()});
                }
            }
        }

        return result;
    }

    std::vector<double> interaction::convolution_samples(const function &left, const function &right, double start, double end, int samples)
    {
        if (samples <= 0)
        {
            return {};
        }

        const double step = samples == 1 ? 0.0 : (end - start) / static_cast<double>(samples - 1);

        std::vector<double> left_samples;
        std::vector<double> right_samples;
        left_samples.reserve(static_cast<std::size_t>(samples));
        right_samples.reserve(static_cast<std::size_t>(samples));

        for (int i = 0; i < samples; ++i)
        {
            const double x = start + step * static_cast<double>(i);
            left_samples.push_back(std::real(left.evaluate(x)));
            right_samples.push_back(std::real(right.evaluate(x)));
        }

        std::vector<double> result(static_cast<std::size_t>(2 * samples - 1), 0.0);

        for (int i = 0; i < samples; ++i)
        {
            for (int j = 0; j < samples; ++j)
            {
                result[static_cast<std::size_t>(i + j)] += left_samples[static_cast<std::size_t>(i)] * right_samples[static_cast<std::size_t>(j)] * step;
            }
        }

        return result;
    }
}