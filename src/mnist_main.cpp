#include "network/network.hpp"
#include "network/mnist_loader.hpp"

#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        std::cerr << "Usage: mnist_train <train-images> <train-labels> "
                     "<test-images> <test-labels>\n";
        return 1;
    }

    try
    {
        std::cout << "Loading MNIST...\n";

        const auto train = analog::network::mnist_loader::load(argv[1], argv[2]);
        const auto test  = analog::network::mnist_loader::load(argv[3], argv[4]);

        std::cout << "Loaded " << train.images.size() << " training samples, "
                  << test.images.size() << " test samples.\n\n";

        analog::network::network net{64, 10, 0.01};

        constexpr int    epochs = 10;
        constexpr double lr     = 0.01;

        net.train(train, test, epochs, lr);
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}
