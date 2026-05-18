#include "layer.hpp"

#include "../core/wave.hpp"

#include <algorithm>
#include <random>

namespace analog::network
{
    layer::layer(int n_inputs, double init_scale)
        : n_inputs_(n_inputs),
          weights_(n_inputs),
          pre_relu_(n_inputs, 0.0),
          cached_x_(n_inputs, 0.0)
    {
        std::mt19937 rng{std::random_device{}()};
        std::uniform_real_distribution<double> dist(-init_scale, init_scale);

        for (auto& w : weights_)
            w = dist(rng);
    }

    std::vector<double> layer::forward(const analog::function& input)
    {
        std::vector<double> h(n_inputs_);

        for (int i = 0; i < n_inputs_; ++i)
        {
            const analog::wave* w = input.get_wave(static_cast<double>(i));
            const double x = w ? w->amplitude() : 0.0;

            cached_x_[i] = x;
            pre_relu_[i] = x * weights_[i];
            h[i]         = std::max(0.0, pre_relu_[i]);
        }

        return h;
    }

    std::vector<double> layer::backward(const std::vector<double>& grad_h, double learning_rate)
    {
        std::vector<double> grad_x(n_inputs_, 0.0);

        for (int i = 0; i < n_inputs_; ++i)
        {
            const double relu_mask = pre_relu_[i] > 0.0 ? 1.0 : 0.0;
            const double grad_pre  = grad_h[i] * relu_mask;

            // grad_x before weight update
            grad_x[i] = grad_pre * weights_[i];

            weights_[i] -= learning_rate * grad_pre * cached_x_[i];
        }

        return grad_x;
    }

    analog::function layer::weights_as_function() const
    {
        analog::function f;

        for (int i = 0; i < n_inputs_; ++i)
            f.add_wave(analog::wave{static_cast<double>(i), weights_[i], 0.0});

        return f;
    }
}
