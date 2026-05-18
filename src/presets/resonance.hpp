#pragma once

#include "preset.hpp"

namespace analog
{
    class resonance : public preset
    {
    public:
        resonance(double center_frequency, double bandwidth);

        double evaluate(double frequency) const override;

    private:
        double center_frequency_;
        double bandwidth_;
    };
}
