#pragma once

#include "../core/function.hpp"

#include <vector>

namespace analog::network
{
    // 2D spatial resonator: detects a rectangular patch of the image.
    // Input waves must encode pixel position as freq = (row*28 + col) / 783.
    // The response is H(r,c) = H_r(r/27) * H_c(c/27), both Lorentzian.
    // Output: ⟨image, H⟩ = Σ_{r,c} pixel(r,c)/255 * H_r(r/27) * H_c(c/27)
    class resonator_node_2d
    {
    public:
        resonator_node_2d(double c_r, double b_r, double c_c, double b_c, int n_inputs);

        double              forward(const analog::function& input);
        std::vector<double> backward(double grad_out, double learning_rate);

        double c_r() const { return c_r_; }
        double c_c() const { return c_c_; }
        double b_r() const { return b_r_; }
        double b_c() const { return b_c_; }

    private:
        double c_r_, b_r_;
        double c_c_, b_c_;
        int    n_inputs_;

        // Cached state from last forward pass
        std::vector<double> A_;    // input amplitudes
        std::vector<double> H_r_;  // row Lorentzian values
        std::vector<double> H_c_;  // column Lorentzian values
        std::vector<int>    row_;  // decoded row index per wave
        std::vector<int>    col_;  // decoded column index per wave
    };
}
