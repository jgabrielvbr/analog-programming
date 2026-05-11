#include "core/function.hpp"
#include "core/wave.hpp"
#include "operators/amplitude_scale_operator.hpp"
#include "operators/frequency_scale_operator.hpp"
#include "operators/phase_shift_operator.hpp"

#include <complex>
#include <iostream>
#include <numbers>

namespace
{
    void print_evaluations(const char* label, const analog::function& signal)
    {
        std::cout << label << '\n';

        for (int i = 0; i < 4; ++i)
        {
            const double x = static_cast<double>(i);
            const std::complex<double> value = signal.evaluate(x);

            std::cout << "x = " << x << ", value = " << value << '\n';
        }

        std::cout << '\n';
    }
}

int main()
{
    std::cout << "Analog computation playground\n";

    constexpr double pi = std::numbers::pi;

    analog::function signal;
    signal.add_wave(analog::wave{1.0, 1.0});
    signal.add_wave(analog::wave{2.0, 0.5, pi / 4.0});
    signal.add_wave(analog::wave{4.0, 0.25});

    const analog::amplitude_scale_operator amplitude_scale{2.0};
    const analog::function amplitude_scaled = amplitude_scale.apply(signal);

    const analog::phase_shift_operator phase_shift{pi / 2.0};
    const analog::function phase_shifted = phase_shift.apply(amplitude_scaled);

    const analog::frequency_scale_operator frequency_scale{0.5};
    const analog::function frequency_scaled = frequency_scale.apply(phase_shifted);

    print_evaluations("original", signal);
    print_evaluations("after amplitude scale", amplitude_scaled);
    print_evaluations("after phase shift", phase_shifted);
    print_evaluations("after frequency scale", frequency_scaled);

    return 0;
}
