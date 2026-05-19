#pragma once

#include "../encoder.hpp"

namespace analog
{
    class mnist_encoder : public encoder
    {
    public:
        function encode(const sample &input) const override;
    };
}
