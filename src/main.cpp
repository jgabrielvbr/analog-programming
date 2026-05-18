#include "core/function.hpp"
#include "core/wave.hpp"
#include "transforms/amplitude_scale.hpp"
#include "transforms/frequency_scale.hpp"
#include "transforms/phase_shift.hpp"
#include "transforms/preset_transform.hpp"
#include "interactions/convolution.hpp"
#include "interactions/mixing.hpp"
#include "interactions/difference.hpp"
#include "presets/low_pass.hpp"
#include "presets/gaussian.hpp"
#include "presets/resonance.hpp"

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

    void print_waves(const char* label, const analog::function& signal)
    {
        std::cout << label << '\n';

        for (const auto& wave : signal.waves())
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
    std::cout << "Analog computation playground\n\n";

    constexpr double pi = std::numbers::pi;

    analog::function signal;
    signal.add_wave(analog::wave{1.0, 1.0,  0.0});
    signal.add_wave(analog::wave{2.0, 0.5,  pi / 4.0});
    signal.add_wave(analog::wave{4.0, 0.25, 0.0});
    signal.add_wave(analog::wave{8.0, 0.1,  pi / 6.0});

    print_waves("original waves", signal);

    // --- existing transforms ---

    const analog::amplitude_scale amplitude_scale{2.0};
    const analog::function amplitude_scaled = amplitude_scale.apply(signal);

    const analog::phase_shift phase_shift{pi / 2.0};
    const analog::function phase_shifted = phase_shift.apply(amplitude_scaled);

    const analog::frequency_scale frequency_scale{0.5};
    const analog::function frequency_scaled = frequency_scale.apply(phase_shifted);

    print_evaluations("original", signal);
    print_evaluations("after amplitude scale", amplitude_scaled);
    print_evaluations("after phase shift", phase_shifted);
    print_evaluations("after frequency scale", frequency_scaled);

    // --- preset as function + convolution ---

    // low_pass: build H as a function, then convolve explicitly
    const analog::low_pass lp{3.0};
    const analog::function lp_function = lp.to_function(signal);

    print_waves("low_pass H(ω) as function (cutoff=3)", lp_function);

    const analog::convolution conv;
    const analog::function lp_filtered = conv.apply(signal, lp_function);

    print_waves("signal convolved with low_pass H(ω)", lp_filtered);

    // preset_transform wraps the same operation as a convenience
    const analog::preset_transform lp_transform{lp};
    const analog::function lp_via_transform = lp_transform.apply(signal);

    print_waves("same result via preset_transform.apply", lp_via_transform);

    // apply_select: binary frequency selection path (no H evaluation)
    const analog::function lp_selected = lp_transform.apply_select(signal);

    print_waves("low_pass apply_select (binary path, cutoff=3)", lp_selected);

    // gaussian: smooth rolloff — H never reaches zero
    const analog::gaussian gauss{2.0};
    const analog::function gauss_filtered = conv.apply(signal, gauss.to_function(signal));

    print_waves("signal convolved with gaussian H(ω) (sigma=2)", gauss_filtered);

    // resonance: Lorentzian peak around center frequency
    const analog::resonance res{2.0, 0.5};
    const analog::function res_filtered = conv.apply(signal, res.to_function(signal));

    print_waves("signal convolved with resonance H(ω) (center=2, bandwidth=0.5)", res_filtered);

    // --- mixing: sum and difference sidebands ---

    analog::function carrier;
    carrier.add_wave(analog::wave{10.0, 1.0, 0.0});

    analog::function modulator;
    modulator.add_wave(analog::wave{1.0, 0.5, 0.0});
    modulator.add_wave(analog::wave{3.0, 0.5, 0.0});

    const analog::mixing mix;
    const analog::function mixed = mix.apply(carrier, modulator);

    print_waves("mixing: carrier(10) x modulator(1,3) -> sidebands", mixed);

    // --- difference: MSE between original and low-pass filtered ---

    const analog::difference diff;
    const double mse = diff.measure(signal, lp_filtered, 0.0, 4.0, 256);

    std::cout << "difference MSE (signal vs low_pass filtered): " << mse << "\n\n";

    return 0;
}
