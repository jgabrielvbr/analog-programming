#include "transform.hpp"

namespace analog
{
    function transform::amplitude_scale(const function& input, double scale)
    {
        function output;

        for (const auto& wave : input.waves())
        {
            output.add_wave({wave.frequency(), wave.amplitude() * scale, wave.phase()});
        }

        return output;
    }

    function transform::phase_shift(const function& input, double phase)
    {
        function output;

        for (const auto& wave : input.waves())
        {
            output.add_wave({wave.frequency(), wave.amplitude(), wave.phase() + phase});
        }

        return output;
    }

    function transform::frequency_shift(const function& input, double delta_frequency)
    {
        function output;

        for (const auto& wave : input.waves())
        {
            output.add_wave({wave.frequency() + delta_frequency, wave.amplitude(), wave.phase()});
        }

        return output;
    }

    function transform::frequency_scale(const function& input, double scale)
    {
        function output;

        for (const auto& wave : input.waves())
        {
            output.add_wave({wave.frequency() * scale, wave.amplitude(), wave.phase()});
        }

        return output;
    }

    function transform::time_shift(const function& input, double shift)
    {
        function output;

        for (const auto& wave : input.waves())
        {
            output.add_wave({
                wave.frequency(),
                wave.amplitude(),
                wave.phase() - wave.frequency() * shift
            });
        }

        return output;
    }

    function transform::conjugate(const function& input)
    {
        function output;

        for (const auto& wave : input.waves())
        {
            output.add_wave({
                -wave.frequency(),
                wave.amplitude(),
                -wave.phase()
            });
        }

        return output;
    }
}