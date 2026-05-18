#pragma once

#include "../core/function.hpp"

#include <vector>

namespace analog::network
{
    class layer
    {
    public:
        explicit layer(int n_inputs = 784, double init_scale = 0.01);

        std::vector<double> forward(const analog::function& input);
        std::vector<double> backward(const std::vector<double>& grad_h, double learning_rate);

        analog::function weights_as_function() const;

    private:
        int                 n_inputs_;
        std::vector<double> weights_;
        std::vector<double> pre_relu_;
        std::vector<double> cached_x_;
    };
}
