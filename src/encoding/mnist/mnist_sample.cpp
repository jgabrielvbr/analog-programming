#include "mnist_sample.hpp"

namespace analog
{
    mnist_sample::mnist_sample(std::vector<std::uint8_t> pixels, int label) : pixels_(std::move(pixels)), label_(label)
    {
    }

    const std::vector<std::uint8_t> &mnist_sample::pixels() const
    {
        return pixels_;
    }

    int mnist_sample::label() const
    {
        return label_;
    }

    int mnist_sample::width() const
    {
        return width_;
    }

    int mnist_sample::height() const
    {
        return height_;
    }
}