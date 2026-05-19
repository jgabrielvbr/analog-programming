#pragma once

#include "mnist_sample.hpp"

#include <cstddef>
#include <filesystem>
#include <vector>

namespace analog
{
    std::vector<mnist_sample> load_mnist_samples(
        const std::filesystem::path& images_path,
        const std::filesystem::path& labels_path,
        std::size_t limit = 0);
}
