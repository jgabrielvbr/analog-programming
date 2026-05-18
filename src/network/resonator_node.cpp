#include "resonator_node.hpp"

#include <algorithm>
#include <cmath>

namespace analog::network
{
    resonator_node::resonator_node(double center_frequency, double bandwidth, int n_inputs)
        : c_(center_frequency),
          b_(bandwidth),
          n_inputs_(n_inputs),
          omega_(n_inputs, 0.0),
          A_(n_inputs, 0.0),
          H_(n_inputs, 0.0)
    {
    }

    double resonator_node::forward(const analog::function& input)
    {
        const auto& waves = input.waves();
        const int n = static_cast<int>(waves.size());

        omega_.resize(n);
        A_.resize(n);
        H_.resize(n);

        double out = 0.0;

        for (int j = 0; j < n; ++j)
        {
            const double omega = waves[j].frequency();
            const double amp   = waves[j].amplitude();
            const double D     = (omega - c_) / b_;
            const double H     = 1.0 / (1.0 + D * D);

            omega_[j] = omega;
            A_[j]     = amp;
            H_[j]     = H;

            out += amp * H;
        }

        return out;
    }

    std::vector<double> resonator_node::backward(double grad_out, double learning_rate)
    {
        const int n = static_cast<int>(omega_.size());

        std::vector<double> grad_A(n, 0.0);
        double grad_c = 0.0;
        double grad_b = 0.0;

        for (int j = 0; j < n; ++j)
        {
            // ∂out/∂A_j = H_j
            grad_A[j] = grad_out * H_[j];

            // ∂out/∂c  = Σ A_j · ∂H/∂c,  ∂H/∂c = 2·D·H²/b
            // ∂out/∂b  = Σ A_j · ∂H/∂b,  ∂H/∂b = 2·D²·H²/b
            const double D    = (omega_[j] - c_) / b_;
            const double H2   = H_[j] * H_[j];
            const double base = grad_out * A_[j];

            grad_c += base * (2.0 * D  * H2 / b_);
            grad_b += base * (2.0 * D * D * H2 / b_);
        }

        // Scale all parameter updates by b² to cancel the 1/b factor in ∂H/∂c and ∂H/∂b,
        // keeping Δc and Δb proportional to b regardless of bandwidth.
        c_ -= learning_rate * b_ * b_ * grad_c;
        b_ -= learning_rate * b_ * b_ * grad_b;

        c_ = std::max(0.0, std::min(1.0, c_));
        b_ = std::max(b_, 0.001);

        return grad_A;
    }
}
