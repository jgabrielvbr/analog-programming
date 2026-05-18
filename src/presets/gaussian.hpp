#pragma once

#include "preset.hpp"

namespace analog
{
    class gaussian : public preset
    {
    public:
        explicit gaussian(double sigma);

        double evaluate(double frequency) const override;

    private:
        double sigma_;
    };
}
