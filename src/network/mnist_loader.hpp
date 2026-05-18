#pragma once

#include "../core/function.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace analog::network
{
    struct mnist_dataset
    {
        std::vector<analog::function> images;
        std::vector<uint8_t>          labels;
    };

    class mnist_loader
    {
    public:
        static mnist_dataset load(const std::string& images_path,
                                  const std::string& labels_path);

    private:
        static uint32_t read_be_uint32(std::ifstream& f);
    };
}
