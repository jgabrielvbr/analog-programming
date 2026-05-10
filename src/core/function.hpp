#pragma once

#include <complex>
#include <vector>

#include "wave.hpp"

namespace analog
{
    class function
    {
        public:
            function() = default;

            void add_wave(const wave& wave);

            std::complex<double> evaluate(double x) const;
            
            double evaluate_real(double x) const;
            double evaluate_imag(double x) const;

            const std::vector<wave>& waves() const;

        private:
            std::vector<wave> waves_;
    };
}