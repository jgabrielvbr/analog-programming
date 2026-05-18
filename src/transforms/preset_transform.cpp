#include "preset_transform.hpp"
#include "../interactions/convolution.hpp"

namespace analog
{
    preset_transform::preset_transform(const preset& preset) : preset_(preset) {}

    analog::function preset_transform::apply(const analog::function& input) const
    {
        const analog::convolution conv;
        return conv.apply(input, preset_.to_function(input));
    }

    analog::function preset_transform::apply_select(const analog::function& input) const
    {
        analog::function output;

        for (const auto& wave : input.waves())
        {
            if (preset_.select(wave.frequency()))
                output.add_wave(wave);
        }

        return output;
    }
}
