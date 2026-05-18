#include "resonator_layer_2d.hpp"

#include "../core/wave.hpp"

namespace analog::network
{
    resonator_layer_2d::resonator_layer_2d(int n_nodes, int n_inputs,
                                           int n_r, int n_c,
                                           double bw_r, double bw_c)
        : n_nodes_(n_nodes), n_inputs_(n_inputs)
    {
        nodes_.reserve(n_nodes);
        for (int ir = 0; ir < n_r; ++ir)
        {
            for (int ic = 0; ic < n_c; ++ic)
            {
                const double c_r = (ir + 0.5) / n_r;
                const double c_c = (ic + 0.5) / n_c;
                nodes_.emplace_back(c_r, bw_r, c_c, bw_c, n_inputs);
            }
        }
    }

    analog::function resonator_layer_2d::forward(const analog::function& input)
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

    std::vector<double> resonator_layer_2d::backward(const std::vector<double>& delta,
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
