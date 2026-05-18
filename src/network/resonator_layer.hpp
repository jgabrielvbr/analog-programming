#pragma once

#include "resonator_node.hpp"
#include "../core/function.hpp"

#include <vector>

namespace analog::network
{
    class resonator_layer
    {
    public:
        resonator_layer(int n_nodes, int n_inputs, double freq_min, double freq_max, double bw_init);

        analog::function    forward(const analog::function& input);
        std::vector<double> backward(const std::vector<double>& delta, double learning_rate);

        int n_nodes()  const { return n_nodes_;  }
        int n_inputs() const { return n_inputs_; }

    private:
        int n_nodes_;
        int n_inputs_;
        std::vector<resonator_node> nodes_;
    };
}
