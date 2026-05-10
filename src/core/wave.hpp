#pragma once

#include <complex>

namespace analog
{
    class wave
    {
        public:
            wave(double frequency, double amplitude);
        
            double frequency() const;
            double amplitude() const;
        
            std::complex<double> evaluate(double x) const;
        
        private:
            double frequency_;
            double amplitude_;
    };
}