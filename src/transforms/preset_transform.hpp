#pragma once

#include "transform.hpp"
#include "../presets/preset.hpp"

namespace analog
{
    class preset_transform : public transform
    {
    public:
        explicit preset_transform(const preset& preset);

        // Convolves input with H(ω) = preset.to_function(input)
        analog::function apply(const analog::function& input) const override;

        // Keeps only waves where preset.select(frequency) is true
        analog::function apply_select(const analog::function& input) const;

    private:
        const preset& preset_;
    };
}
