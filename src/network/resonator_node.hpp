#pragma once

#include "../core/function.hpp"

#include <vector>

namespace analog::network
{
    class resonator_node
    {
    public:
        resonator_node(double center_frequency, double bandwidth, int n_inputs);

        // Returns ⟨input, H_k⟩ = Σ_j A_j · H_k(ω_j)  (signed inner product)
        double forward(const analog::function& input);
        // Returns ∂L/∂A_j for each input wave; updates center and bandwidth
        std::vector<double> backward(double grad_out, double learning_rate);

        double center() const { return c_; }
        double bw()     const { return b_; }

    private:
        double c_, b_;
        int    n_inputs_;

        std::vector<double> omega_;
        std::vector<double> A_;
        std::vector<double> H_;
    };
}
