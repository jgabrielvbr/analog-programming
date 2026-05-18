#include "resonator_layer.hpp"

#include "../core/wave.hpp"

namespace analog::network
{
    resonator_layer::resonator_layer(int n_nodes, int n_inputs,
                                     double freq_min, double freq_max, double bw_init)
        : n_nodes_(n_nodes), n_inputs_(n_inputs)
    {
        nodes_.reserve(n_nodes);
        for (int k = 0; k < n_nodes; ++k)
        {
            const double c = freq_min + (freq_max - freq_min) * (k + 0.5) / n_nodes;
            nodes_.emplace_back(c, bw_init, n_inputs);
        }
    }

    analog::function resonator_layer::forward(const analog::function& input)
    {
        analog::function output;
        for (int k = 0; k < n_nodes_; ++k)
        {
            const double amp  = nodes_[k].forward(input);
            const double freq = static_cast<double>(k) / n_nodes_;
            output.add_wave(analog::wave{freq, amp, 0.0});
        }
        return output;
    }

    std::vector<double> resonator_layer::backward(const std::vector<double>& delta,
                                                   double learning_rate)
    {
        std::vector<double> grad_amp(n_inputs_, 0.0);

        for (int k = 0; k < n_nodes_; ++k)
        {
            const std::vector<double> node_grad = nodes_[k].backward(delta[k], learning_rate);
            for (int j = 0; j < static_cast<int>(node_grad.size()); ++j)
                grad_amp[j] += node_grad[j];
        }

        return grad_amp;
    }
}
