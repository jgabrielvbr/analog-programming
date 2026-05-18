#include "network.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>

namespace analog::network
{
    static int n_side(int n_hidden)
    {
        return static_cast<int>(std::round(std::sqrt(static_cast<double>(n_hidden))));
    }

    network::network(int n_hidden, int n_classes, double init_scale)
        : hidden_(n_side(n_hidden) * n_side(n_hidden), 784,
                  n_side(n_hidden), n_side(n_hidden),
                  1.0 / n_side(n_hidden), 1.0 / n_side(n_hidden)),
          output_(n_classes, n_side(n_hidden) * n_side(n_hidden), init_scale)
    {
    }

    std::vector<double> network::softmax(const std::vector<double>& scores)
    {
        const double max_score = *std::max_element(scores.begin(), scores.end());

        std::vector<double> probs(scores.size());
        double sum = 0.0;

        for (std::size_t k = 0; k < scores.size(); ++k)
        {
            probs[k] = std::exp(scores[k] - max_score);
            sum += probs[k];
        }

        for (auto& p : probs) p /= sum;
        return probs;
    }

    double network::cross_entropy(const std::vector<double>& probs, int label)
    {
        return -std::log(probs[label] + 1e-12);
    }

    std::vector<double> network::compute_delta(const std::vector<double>& probs, int label)
    {
        std::vector<double> delta = probs;
        delta[label] -= 1.0;
        return delta;
    }

    std::pair<double, int> network::train_one(const analog::function& image,
                                               int label, double learning_rate)
    {
        // Stage 1: image → 64 spatial patch scores (always positive)
        const analog::function h_fn = hidden_.forward(image);
        const auto& h_waves = h_fn.waves();
        std::vector<double> h(h_waves.size());
        for (std::size_t k = 0; k < h_waves.size(); ++k)
            h[k] = h_waves[k].amplitude();

        // Stage 2: signed linear combination → 10 class logits
        const std::vector<double> logits = output_.forward(h);
        const std::vector<double> probs  = softmax(logits);
        const double loss = cross_entropy(probs, label);
        const int pred = static_cast<int>(
            std::max_element(logits.begin(), logits.end()) - logits.begin());

        // Backward
        const std::vector<double> delta  = compute_delta(probs, label);
        const std::vector<double> grad_h = output_.backward(delta, learning_rate);
        hidden_.backward(grad_h, learning_rate);

        return {loss, pred};
    }

    int network::predict(const analog::function& image)
    {
        const analog::function h_fn = hidden_.forward(image);
        const auto& h_waves = h_fn.waves();
        std::vector<double> h(h_waves.size());
        for (std::size_t k = 0; k < h_waves.size(); ++k)
            h[k] = h_waves[k].amplitude();

        const std::vector<double> logits = output_.forward(h);
        return output_.predict(logits);
    }

    void network::train(const mnist_dataset& train_data,
                        const mnist_dataset& test_data,
                        int epochs, double learning_rate)
    {
        const int n = static_cast<int>(train_data.images.size());

        std::vector<int> indices(n);
        std::iota(indices.begin(), indices.end(), 0);

        std::mt19937 rng{42};

        for (int epoch = 0; epoch < epochs; ++epoch)
        {
            std::shuffle(indices.begin(), indices.end(), rng);

            double total_loss = 0.0;
            int    correct    = 0;

            for (int idx : indices)
            {
                const auto [loss, pred] = train_one(
                    train_data.images[idx],
                    static_cast<int>(train_data.labels[idx]),
                    learning_rate);

                total_loss += loss;
                if (pred == static_cast<int>(train_data.labels[idx]))
                    ++correct;
            }

            const double avg_loss  = total_loss / n;
            const double train_acc = static_cast<double>(correct) / n * 100.0;

            int test_correct = 0;
            for (std::size_t i = 0; i < test_data.images.size(); ++i)
            {
                if (predict(test_data.images[i]) == static_cast<int>(test_data.labels[i]))
                    ++test_correct;
            }

            const double test_acc = static_cast<double>(test_correct) /
                                    static_cast<double>(test_data.images.size()) * 100.0;

            std::cout << "epoch " << (epoch + 1)
                      << "  loss=" << avg_loss
                      << "  train=" << train_acc << "%"
                      << "  test=" << test_acc << "%\n"
                      << std::flush;
        }
    }
}
