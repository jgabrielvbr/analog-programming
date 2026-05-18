#pragma once

#include "../core/function.hpp"

#include <vector>

namespace analog::network
{
    class input_layer
    {
    public:
        explicit input_layer(int n_pixels = 784, double init_scale = 0.01);

        // raw_image: freq = pixel index, amp = pixel_value / 255 (loader format)
        // Returns F_in: freq = brightness, amp = W_i (learnable per position)
        analog::function forward(const analog::function& raw_image);
        void             backward(const std::vector<double>& grad_amp, double learning_rate);

        int n_pixels() const { return n_pixels_; }

    private:
        int                 n_pixels_;
        std::vector<double> weights_;
        std::vector<int>    pixel_wave_pos_;  // position of pixel i's wave in the last F_in
    };
}
