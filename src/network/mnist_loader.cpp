#include "mnist_loader.hpp"

#include "../core/wave.hpp"

#include <fstream>
#include <stdexcept>

namespace analog::network
{
    uint32_t mnist_loader::read_be_uint32(std::ifstream& f)
    {
        uint8_t b[4];
        f.read(reinterpret_cast<char*>(b), 4);
        return (static_cast<uint32_t>(b[0]) << 24)
             | (static_cast<uint32_t>(b[1]) << 16)
             | (static_cast<uint32_t>(b[2]) <<  8)
             |  static_cast<uint32_t>(b[3]);
    }

    mnist_dataset mnist_loader::load(const std::string& images_path, const std::string& labels_path)
    {
        std::ifstream img_f(images_path, std::ios::binary);
        std::ifstream lbl_f(labels_path, std::ios::binary);

        if (!img_f) throw std::runtime_error("Cannot open images file: " + images_path);
        if (!lbl_f) throw std::runtime_error("Cannot open labels file: " + labels_path);

        const uint32_t img_magic = read_be_uint32(img_f);
        if (img_magic != 0x00000803)
            throw std::runtime_error("Invalid images magic number");

        const uint32_t lbl_magic = read_be_uint32(lbl_f);
        if (lbl_magic != 0x00000801)
            throw std::runtime_error("Invalid labels magic number");

        const uint32_t n_images = read_be_uint32(img_f);
        const uint32_t n_labels = read_be_uint32(lbl_f);

        if (n_images != n_labels)
            throw std::runtime_error("Image and label counts do not match");

        const uint32_t rows = read_be_uint32(img_f);
        const uint32_t cols = read_be_uint32(img_f);
        const uint32_t n_pixels = rows * cols;

        mnist_dataset dataset;
        dataset.images.reserve(n_images);
        dataset.labels.resize(n_images);

        std::vector<uint8_t> buf(n_pixels);

        for (uint32_t n = 0; n < n_images; ++n)
        {
            img_f.read(reinterpret_cast<char*>(buf.data()), n_pixels);

            analog::function image;
            for (uint32_t i = 0; i < n_pixels; ++i)
                image.add_wave(analog::wave{
                    static_cast<double>(i) / (n_pixels - 1),  // freq = position address ∈ [0,1]
                    buf[i] / 255.0,                            // amp  = normalised brightness
                    0.0
                });

            dataset.images.push_back(std::move(image));
        }

        lbl_f.read(reinterpret_cast<char*>(dataset.labels.data()), n_labels);

        return dataset;
    }
}
