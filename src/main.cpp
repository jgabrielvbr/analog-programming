#include "core/function.hpp"
#include "core/wave.hpp"
#include "filters/low_pass_filter.hpp"

#include <complex>
#include <iostream>
#include <numbers>

int main()
{
    std::cout << "Analog computation playground\n";

    constexpr double pi = std::numbers::pi;

    analog::function signal;
    signal.add_wave(analog::wave{1.0, 1.0});
    signal.add_wave(analog::wave{3.0, 0.5});
    signal.add_wave(analog::wave{8.0, 0.25});

    const analog::low_pass_filter filter{3.0};
    const analog::function filtered = filter.apply(signal);

    for (int i = 0; i < 6; ++i)
    {
        const double x = 2.0 * pi * i / 5.0;
        const std::complex<double> original_value = signal.evaluate(x);
        const std::complex<double> filtered_value = filtered.evaluate(x);

        std::cout << "x = " << x
                  << ", original = " << original_value
                  << ", filtered = " << filtered_value << '\n';
    }

    return 0;
}
