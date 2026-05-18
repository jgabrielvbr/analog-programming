#pragma once

#include "resonator_node_2d.hpp"
#include "../core/function.hpp"

#include <vector>

namespace analog::network
{
    // A layer of 2D resonator nodes arranged in an n_r × n_c grid.
    // Each node covers a distinct spatial patch of the image.
    // n_nodes must equal n_r * n_c.
    class resonator_layer_2d
    {
    public:
        resonator_layer_2d(int n_nodes, int n_inputs,
                           int n_r, int n_c,
                           double bw_r, double bw_c);

        analog::function    forward(const analog::function& input);
        std::vector<double> backward(const std::vector<double>& delta, double learning_rate);

        int n_nodes()  const { return n_nodes_;  }
        int n_inputs() const { return n_inputs_; }

    private:
        int n_nodes_;
        int n_inputs_;
        std::vector<resonator_node_2d> nodes_;
    };
}
