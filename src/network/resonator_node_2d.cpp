#include "resonator_node_2d.hpp"

#include <algorithm>
#include <cmath>

namespace analog::network
{
    resonator_node_2d::resonator_node_2d(double c_r, double b_r,
                                         double c_c, double b_c, int n_inputs)
        : c_r_(c_r), b_r_(b_r), c_c_(c_c), b_c_(b_c), n_inputs_(n_inputs),
          A_(n_inputs, 0.0), H_r_(n_inputs, 0.0), H_c_(n_inputs, 0.0),
          row_(n_inputs, 0), col_(n_inputs, 0)
    {
    }

    double resonator_node_2d::forward(const analog::function& input)
    {
        const auto& waves = input.waves();
        const int n = static_cast<int>(waves.size());

        A_.resize(n); H_r_.resize(n); H_c_.resize(n);
        row_.resize(n); col_.resize(n);

        double out = 0.0;

        for (int j = 0; j < n; ++j)
        {
            // Decode 2D position from frequency = (row*28 + col) / 783
            const int   idx = static_cast<int>(std::round(waves[j].frequency() * 783.0));
            const int   r   = idx / 28;
            const int   c   = idx % 28;
            const double amp = waves[j].amplitude();

            const double Dr = (r / 27.0 - c_r_) / b_r_;
            const double Dc = (c / 27.0 - c_c_) / b_c_;
            const double Hr = 1.0 / (1.0 + Dr * Dr);
            const double Hc = 1.0 / (1.0 + Dc * Dc);

            A_[j]   = amp;
            H_r_[j] = Hr;
            H_c_[j] = Hc;
            row_[j] = r;
            col_[j] = c;

            out += amp * Hr * Hc;
        }

        return out;
    }

    std::vector<double> resonator_node_2d::backward(double grad_out, double learning_rate)
    {
        const int n = static_cast<int>(A_.size());

        std::vector<double> grad_A(n, 0.0);
        double grad_cr = 0.0, grad_br = 0.0;
        double grad_cc = 0.0, grad_bc = 0.0;

        for (int j = 0; j < n; ++j)
        {
            const double Hr = H_r_[j];
            const double Hc = H_c_[j];

            // ∂out/∂A_j = Hr * Hc
            grad_A[j] = grad_out * Hr * Hc;

            const double omega_r = row_[j] / 27.0;
            const double omega_c = col_[j] / 27.0;
            const double Dr = (omega_r - c_r_) / b_r_;
            const double Dc = (omega_c - c_c_) / b_c_;

            // ∂Hr/∂c_r = 2*Dr*Hr²/b_r,  ∂Hr/∂b_r = 2*Dr²*Hr²/b_r
            // ∂Hc/∂c_c = 2*Dc*Hc²/b_c,  ∂Hc/∂b_c = 2*Dc²*Hc²/b_c
            const double base = grad_out * A_[j];

            grad_cr += base * Hc * (2.0 * Dr  * Hr * Hr / b_r_);
            grad_br += base * Hc * (2.0 * Dr*Dr * Hr * Hr / b_r_);
            grad_cc += base * Hr * (2.0 * Dc  * Hc * Hc / b_c_);
            grad_bc += base * Hr * (2.0 * Dc*Dc * Hc * Hc / b_c_);
        }

        // Scale all parameter updates by b² to cancel the 1/b factor in ∂H/∂c and ∂H/∂b,
        // keeping Δc and Δb proportional to b regardless of bandwidth.
        c_r_ -= learning_rate * b_r_ * b_r_ * grad_cr;
        c_c_ -= learning_rate * b_c_ * b_c_ * grad_cc;
        b_r_ -= learning_rate * b_r_ * b_r_ * grad_br;
        b_c_ -= learning_rate * b_c_ * b_c_ * grad_bc;

        c_r_ = std::max(0.0, std::min(1.0, c_r_));
        c_c_ = std::max(0.0, std::min(1.0, c_c_));
        b_r_ = std::max(b_r_, 0.01);
        b_c_ = std::max(b_c_, 0.01);

        return grad_A;
    }
}
