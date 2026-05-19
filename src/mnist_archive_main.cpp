#include "encoding/mnist/mnist_encoder.hpp"
#include "encoding/mnist/mnist_loader.hpp"
#include "kernels/archive_builder.hpp"

#include <array>
#include <cstddef>
#include <exception>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

namespace
{
    using confusion_matrix = std::array<std::array<int, 10>, 10>;

    std::size_t parse_limit(const char* value)
    {
        const std::string text(value);
        std::size_t position = 0;
        const unsigned long long parsed = std::stoull(text, &position);

        if (position != text.size())
        {
            throw std::runtime_error("limits must be non-negative integers");
        }

        return static_cast<std::size_t>(parsed);
    }

    void require_digit_archive(const analog::archive& responses)
    {
        std::array<bool, 10> present = {};

        for (const auto& kernel : responses.kernels())
        {
            const int label = kernel.label();

            if (label >= 0 && label < static_cast<int>(present.size()))
            {
                present[static_cast<std::size_t>(label)] = true;
            }
        }

        for (std::size_t digit = 0; digit < present.size(); ++digit)
        {
            if (!present[digit])
            {
                throw std::runtime_error("archive is missing digit response " + std::to_string(digit));
            }
        }
    }

    void print_confusion_matrix(const confusion_matrix& matrix)
    {
        std::cout << "spectral difference confusion matrix\n";
        std::cout << "actual/pred";

        for (int predicted = 0; predicted < 10; ++predicted)
        {
            std::cout << std::setw(5) << predicted;
        }

        std::cout << '\n';

        for (int actual = 0; actual < 10; ++actual)
        {
            std::cout << std::setw(11) << actual;

            for (int predicted = 0; predicted < 10; ++predicted)
            {
                std::cout << std::setw(5) << matrix[static_cast<std::size_t>(actual)][static_cast<std::size_t>(predicted)];
            }

            std::cout << '\n';
        }
    }

    double accuracy(std::size_t correct, std::size_t total)
    {
        if (total == 0)
        {
            return 0.0;
        }

        return 100.0 * static_cast<double>(correct) / static_cast<double>(total);
    }
}

int main(int argc, char** argv)
{
    try
    {
        if (argc > 3)
        {
            throw std::runtime_error("usage: mnist_archive [train_limit] [test_limit]");
        }

        const std::size_t train_limit = argc >= 2 ? parse_limit(argv[1]) : 0;
        const std::size_t test_limit = argc >= 3 ? parse_limit(argv[2]) : 0;

        const std::filesystem::path train_images = "data/train-images-idx3-ubyte";
        const std::filesystem::path train_labels = "data/train-labels-idx1-ubyte";
        const std::filesystem::path test_images = "data/t10k-images-idx3-ubyte";
        const std::filesystem::path test_labels = "data/t10k-labels-idx1-ubyte";

        const auto train_samples = analog::load_mnist_samples(train_images, train_labels, train_limit);

        if (train_samples.empty())
        {
            throw std::runtime_error("no training samples loaded");
        }

        analog::mnist_encoder encoder;
        analog::archive_builder builder;

        for (const auto& sample : train_samples)
        {
            builder.observe(sample.label(), encoder.encode(sample));
        }

        const analog::archive responses = builder.build();
        require_digit_archive(responses);

        const auto test_samples = analog::load_mnist_samples(test_images, test_labels, test_limit);

        if (test_samples.empty())
        {
            throw std::runtime_error("no test samples loaded");
        }

        confusion_matrix difference_confusion = {};
        std::size_t difference_correct = 0;
        std::size_t correlation_correct = 0;

        for (const auto& sample : test_samples)
        {
            const analog::function input = encoder.encode(sample);
            const int actual = sample.label();
            const int difference_prediction = responses.classify_by_spectral_difference(input);
            const int correlation_prediction = responses.classify_by_spectral_correlation(input);

            if (difference_prediction == actual)
            {
                ++difference_correct;
            }

            if (correlation_prediction == actual)
            {
                ++correlation_correct;
            }

            if (difference_prediction >= 0 && difference_prediction < 10)
            {
                difference_confusion[static_cast<std::size_t>(actual)][static_cast<std::size_t>(difference_prediction)] += 1;
            }
        }

        std::cout << "training samples: " << train_samples.size() << '\n';
        std::cout << "test samples: " << test_samples.size() << '\n';
        std::cout << "archive responses: " << responses.kernels().size() << '\n';
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "spectral difference accuracy: " << accuracy(difference_correct, test_samples.size()) << "%\n";
        std::cout << "spectral correlation accuracy: " << accuracy(correlation_correct, test_samples.size()) << "%\n";
        print_confusion_matrix(difference_confusion);
    }
    catch (const std::exception& error)
    {
        std::cerr << "error: " << error.what() << '\n';
        return 1;
    }
}
