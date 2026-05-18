#pragma once

#include <vector>

namespace analog::network
{
    class readout
    {
    public:
        readout(int n_classes = 10, int n_inputs = 784, double init_scale = 0.01);

        std::vector<double> forward(const std::vector<double>& h);
        std::vector<double> backward(const std::vector<double>& delta, double learning_rate);

        int predict(const std::vector<double>& logits) const;

    private:
        int n_classes_;
        int n_inputs_;
        std::vector<std::vector<double>> R_;
        std::vector<double>              cached_h_;
    };
}
