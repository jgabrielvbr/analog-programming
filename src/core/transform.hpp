#pragma once

#include "function.hpp"

namespace analog
{
    class transform
    {
    public:
        static function amplitude_scale(const function &input, double scale);
        static function phase_shift(const function &input, double phase);
        static function frequency_shift(const function &input, double delta_frequency);
        static function frequency_scale(const function &input, double scale);
        static function time_shift(const function &input, double shift);
        static function conjugate(const function &input);
    };
}