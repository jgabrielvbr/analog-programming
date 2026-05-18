#include "readout.hpp"

#include <algorithm>
#include <random>

namespace analog::network
{
    readout::readout(int n_classes, int n_inputs, double init_scale)
        : n_classes_(n_classes),
          n_inputs_(n_inputs),
          R_(n_classes, std::vector<double>(n_inputs)),
          cached_h_(n_inputs, 0.0)
    {
        std::mt19937 rng{std::random_device{}()};
        std::uniform_real_distribution<double> dist(-init_scale, init_scale);

        for (auto& row : R_)
            for (auto& v : row)
                v = dist(rng);
    }

    std::vector<double> readout::forward(const std::vector<double>& h)
    {
        cached_h_ = h;

        std::vector<double> scores(n_classes_, 0.0);

        for (int k = 0; k < n_classes_; ++k)
            for (int i = 0; i < n_inputs_; ++i)
                scores[k] += h[i] * R_[k][i];

        return scores;
    }

    std::vector<double> readout::backward(const std::vector<double>& delta, double learning_rate)
    {
        // Compute grad_h before updating R
        std::vector<double> grad_h(n_inputs_, 0.0);

        for (int i = 0; i < n_inputs_; ++i)
            for (int k = 0; k < n_classes_; ++k)
                grad_h[i] += delta[k] * R_[k][i];

        for (int k = 0; k < n_classes_; ++k)
            for (int i = 0; i < n_inputs_; ++i)
                R_[k][i] -= learning_rate * delta[k] * cached_h_[i];

        return grad_h;
    }

    int readout::predict(const std::vector<double>& logits) const
    {
        return static_cast<int>(
            std::max_element(logits.begin(), logits.end()) - logits.begin());
    }
}
