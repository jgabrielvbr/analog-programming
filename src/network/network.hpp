#pragma once

#include "resonator_layer_2d.hpp"
#include "readout.hpp"
#include "mnist_loader.hpp"

#include "../core/function.hpp"

#include <utility>
#include <vector>

namespace analog::network
{
    // Hybrid resonator + linear-readout network.
    //
    // Stage 1 (hidden, 2D resonators): n_hidden nodes in a sqrt×sqrt spatial grid.
    //   Each node computes ⟨image, H_r⊗H_c⟩ — how bright is patch (c_r, c_c)?
    //   Output: n_hidden spatial feature values (always positive).
    //
    // Stage 2 (readout, linear): 10×n_hidden signed weight matrix.
    //   Combines spatial features with learned sign, enabling multi-lobe patterns.
    //   e.g. "top bright AND center dark" is a valid class feature.
    class network
    {
    public:
        // n_hidden must be a perfect square (e.g. 64 = 8×8)
        network(int n_hidden = 64, int n_classes = 10, double init_scale = 0.01);

        std::pair<double, int> train_one(const analog::function& image, int label,
                                         double learning_rate);
        int predict(const analog::function& image);

        void train(const mnist_dataset& train_data, const mnist_dataset& test_data,
                   int epochs, double learning_rate);

    private:
        resonator_layer_2d hidden_;
        readout             output_;

        static std::vector<double> softmax(const std::vector<double>& scores);
        static double              cross_entropy(const std::vector<double>& probs, int label);
        static std::vector<double> compute_delta(const std::vector<double>& probs, int label);
    };
}
