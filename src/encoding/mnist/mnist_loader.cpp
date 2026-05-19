#include "mnist_loader.hpp"

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace analog
{
    namespace
    {
        constexpr std::uint32_t image_magic = 2051;
        constexpr std::uint32_t label_magic = 2049;
        constexpr std::uint32_t expected_rows = 28;
        constexpr std::uint32_t expected_cols = 28;

        std::ifstream open_binary(const std::filesystem::path& path)
        {
            std::ifstream input(path, std::ios::binary);

            if (!input)
            {
                throw std::runtime_error("failed to open " + path.string());
            }

            return input;
        }

        std::uint32_t read_u32(std::ifstream& input, const std::string& field)
        {
            unsigned char bytes[4] = {};
            input.read(reinterpret_cast<char*>(bytes), 4);

            if (!input)
            {
                throw std::runtime_error("failed to read " + field);
            }

            return (static_cast<std::uint32_t>(bytes[0]) << 24)
                 | (static_cast<std::uint32_t>(bytes[1]) << 16)
                 | (static_cast<std::uint32_t>(bytes[2]) << 8)
                 | static_cast<std::uint32_t>(bytes[3]);
        }
    }

    std::vector<mnist_sample> load_mnist_samples(
        const std::filesystem::path& images_path,
        const std::filesystem::path& labels_path,
        std::size_t limit)
    {
        auto images = open_binary(images_path);
        auto labels = open_binary(labels_path);

        const std::uint32_t images_magic = read_u32(images, "image magic");
        const std::uint32_t image_count = read_u32(images, "image count");
        const std::uint32_t rows = read_u32(images, "image row count");
        const std::uint32_t cols = read_u32(images, "image column count");

        const std::uint32_t labels_magic = read_u32(labels, "label magic");
        const std::uint32_t label_count = read_u32(labels, "label count");

        if (images_magic != image_magic)
        {
            throw std::runtime_error("invalid MNIST image magic in " + images_path.string());
        }

        if (labels_magic != label_magic)
        {
            throw std::runtime_error("invalid MNIST label magic in " + labels_path.string());
        }

        if (image_count != label_count)
        {
            throw std::runtime_error("MNIST image and label counts do not match");
        }

        if (rows != expected_rows || cols != expected_cols)
        {
            throw std::runtime_error("MNIST image shape is not 28x28");
        }

        const std::size_t available = static_cast<std::size_t>(image_count);
        const std::size_t count = limit == 0 ? available : std::min(limit, available);
        const std::size_t image_size = static_cast<std::size_t>(rows) * static_cast<std::size_t>(cols);

        std::vector<mnist_sample> samples;
        samples.reserve(count);

        for (std::size_t i = 0; i < count; ++i)
        {
            std::vector<std::uint8_t> pixels(image_size);
            images.read(reinterpret_cast<char*>(pixels.data()), static_cast<std::streamsize>(pixels.size()));

            if (!images)
            {
                throw std::runtime_error("failed to read MNIST image payload");
            }

            std::uint8_t label = 0;
            labels.read(reinterpret_cast<char*>(&label), 1);

            if (!labels)
            {
                throw std::runtime_error("failed to read MNIST label payload");
            }

            if (label > 9)
            {
                throw std::runtime_error("MNIST label is outside digit range");
            }

            samples.emplace_back(std::move(pixels), static_cast<int>(label));
        }

        return samples;
    }
}
