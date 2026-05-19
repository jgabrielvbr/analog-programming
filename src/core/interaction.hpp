#pragma once

#include "../core/function.hpp"

#include <vector>

namespace analog
{
    class interaction
    {
        public:
            static function superposition(const function &left, const function &right);
            static function subtraction(const function &left, const function &right);
            static function product(const function &left, const function &right);
            static function modulation(const function &carrier, const function &modulator);
            static function mixing(const function &left, const function &right);
            static double correlation(const function &left, const function &right, double start, double end, int samples);
            static double difference(const function &left, const function &right, double start, double end, int samples);
            static function convolution(const function &left, const function &right, double period, double epsilon = 1e-9);
            static std::vector<double> convolution_samples(const function &left, const function &right, double start, double end, int samples);
    };
}