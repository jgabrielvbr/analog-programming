#include "preset.hpp"

namespace analog
{
    bool preset::select(double frequency) const
    {
        return evaluate(frequency) > 0.0;
    }

    analog::function preset::to_function(const analog::function& input) const
    {
        analog::function result;

        for (const auto& wave : input.waves())
            result.add_wave(analog::wave{wave.frequency(), evaluate(wave.frequency()), 0.0});

        return result;
    }
}
