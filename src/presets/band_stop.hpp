#pragma once

#include "preset.hpp"

namespace analog
{
    class band_stop : public preset
    {
    public:
        band_stop(double min_frequency, double max_frequency);

        double evaluate(double frequency) const override;
        bool select(double frequency) const override;

    private:
        double min_frequency_;
        double max_frequency_;
    };
}
