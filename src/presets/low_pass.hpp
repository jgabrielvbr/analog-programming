#pragma once

#include "preset.hpp"

namespace analog
{
    class low_pass : public preset
    {
    public:
        explicit low_pass(double cutoff_frequency);

        double evaluate(double frequency) const override;
        bool select(double frequency) const override;

    private:
        double cutoff_frequency_;
    };
}
