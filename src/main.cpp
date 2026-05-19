#include "core/function.hpp"
#include "core/wave.hpp"
#include "kernels/archive.hpp"
#include "kernels/kernel.hpp"

#include <iostream>
#include <numbers>

int main()
{
    analog::function input;
    input.add_wave({1.0, 1.0});
    input.add_wave({3.0, 0.5});

    analog::function response_zero;
    response_zero.add_wave({1.0, 0.4});

    analog::function response_one;
    response_one.add_wave({1.0, 1.0});
    response_one.add_wave({3.0, 0.5});

    analog::function response_two;
    response_two.add_wave({2.0, 1.0});

    analog::archive responses;
    responses.add_kernel({0, response_zero});
    responses.add_kernel({1, response_one});
    responses.add_kernel({2, response_two});

    const double start = 0.0;
    const double end = 2.0 * std::numbers::pi;
    const int samples = 512;

    // Classification is response matching between functions and response structures.
    const int correlation_prediction = responses.classify_by_correlation(input, start, end, samples);
    const int difference_prediction = responses.classify_by_difference(input, start, end, samples);

    analog::archive empty_responses;

    std::cout << "correlation prediction: " << correlation_prediction << '\n';
    std::cout << "difference prediction: " << difference_prediction << '\n';
    std::cout << "empty archive correlation prediction: "
              << empty_responses.classify_by_correlation(input, start, end, samples)
              << '\n';
    std::cout << "empty archive difference prediction: "
              << empty_responses.classify_by_difference(input, start, end, samples)
              << '\n';
    std::cout << "empty archive spectral correlation prediction: "
              << empty_responses.classify_by_spectral_correlation(input)
              << '\n';
    std::cout << "empty archive spectral difference prediction: "
              << empty_responses.classify_by_spectral_difference(input)
              << '\n';
}
