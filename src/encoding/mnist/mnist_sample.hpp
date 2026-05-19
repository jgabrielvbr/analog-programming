#pragma once

#include "sample.hpp"

#include <cstdint>
#include <vector>

namespace analog
{
    class mnist_sample : public sample
    {
    public:
        mnist_sample(std::vector<std::uint8_t> pixels, int label);

        const std::vector<std::uint8_t> &pixels() const;

        int label() const;

        int width() const;
        int height() const;

    private:
        std::vector<std::uint8_t> pixels_;

        int label_;

        int width_ = 28;
        int height_ = 28;
    };
}