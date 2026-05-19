#include "core/function.hpp"
#include "core/wave.hpp"

#include <complex>
#include <iostream>
#include <numbers>

namespace
{
    void print_evaluations(const char *label, const analog::function &signal)
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

    void print_waves(const char *label, const analog::function &signal)
    {
        std::cout << label << '\n';

        for (const auto &wave : signal.waves())
        {
            std::cout << "  freq = " << wave.frequency()
                      << ", amplitude = " << wave.amplitude()
                      << ", phase = " << wave.phase() << '\n';
        }

        std::cout << '\n';
    }
}

int main()
{
}
