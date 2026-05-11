#pragma once

#include <complex>

namespace analog
{
    class wave
    {
        public:
            wave(double frequency, double amplitude);
            wave(double frequency, double amplitude, double phase);
        
            double frequency() const;
            double amplitude() const;
            double phase() const;
        
            std::complex<double> evaluate(double x) const;
        
        private:
            double frequency_;
            double amplitude_;
            double phase_;
    };
}
