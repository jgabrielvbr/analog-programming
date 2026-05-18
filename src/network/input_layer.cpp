#include "input_layer.hpp"

#include "../core/wave.hpp"

#include <algorithm>
#include <numeric>
#include <random>

namespace analog::network
{
    input_layer::input_layer(int n_pixels, double init_scale)
        : n_pixels_(n_pixels),
          weights_(n_pixels),
          pixel_wave_pos_(n_pixels, 0)
    {
        std::mt19937 rng{std::random_device{}()};
        std::uniform_real_distribution<double> dist(-init_scale, init_scale);
        for (auto& w : weights_) w = dist(rng);
    }

    analog::function input_layer::forward(const analog::function& raw_image)
    {
        const auto& raw_waves = raw_image.waves();  // sorted by freq = pixel index

        // Sort pixel indices by brightness (amplitude), stable to preserve index order for ties
        std::vector<int> order(n_pixels_);
        std::iota(order.begin(), order.end(), 0);
        std::stable_sort(order.begin(), order.end(), [&](int a, int b) {
            return raw_waves[a].amplitude() < raw_waves[b].amplitude();
        });

        // order[j] = pixel index whose wave is at position j in F_in
        for (int j = 0; j < n_pixels_; ++j)
            pixel_wave_pos_[order[j]] = j;

        // Build F_in: freq = brightness, amp = W_i
        // Adding in brightness-sorted order gives clean O(N) insertions into the sorted function
        analog::function f_in;
        for (int j = 0; j < n_pixels_; ++j)
        {
            const int    i          = order[j];
            const double brightness = raw_waves[i].amplitude();
            f_in.add_wave(analog::wave{brightness, weights_[i], 0.0});
        }

        return f_in;
    }

    void input_layer::backward(const std::vector<double>& grad_amp, double learning_rate)
    {
        for (int i = 0; i < n_pixels_; ++i)
            weights_[i] -= learning_rate * grad_amp[pixel_wave_pos_[i]];
    }
}
