#include "mnist_encoder.hpp"

#include "mnist_sample.hpp"

#include "../../core/wave.hpp"

#include <cstddef>

namespace analog
{
    function mnist_encoder::encode(const sample &input) const
    {
        const auto &mnist = dynamic_cast<const mnist_sample &>(input);

        function result;

        const auto &pixels = mnist.pixels();

        for (std::size_t i = 0; i < pixels.size(); ++i)
        {
            const double intensity = static_cast<double>(pixels[i]) / 255.0;

            if (intensity <= 0.0)
            {
                continue;
            }

            const double frequency = static_cast<double>(i + 1);

            result.add_wave({frequency, intensity, 0.0});
        }

        return result;
    }
}
