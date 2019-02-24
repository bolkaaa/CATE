/*
  CATE: Concatenative Audio Processing Application
  Copyright (c) 2019 Liam Wyllie. All rights reserved.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#include <complex>
#include <vector>

#include <fftw3.h>

#include "FFT.hpp"

using std::vector;
using std::complex;

FFT::FFT(uint16_t n)
    : n(n),
      data(new double[n]),
      spectrum(new complex<double>[n / 2 + 1]),
      plan(fftw_plan_dft_r2c_1d(n,
                                reinterpret_cast<double*>(data),
                                reinterpret_cast<fftw_complex*>(spectrum),
                                FFTW_ESTIMATE))
{
}

FFT::~FFT()
{
    delete[] data;
    delete[] spectrum;
}

void FFT::window(double &elem, uint16_t i)
{
    elem *= (1./2) * (1. - std::cos((2. * M_PI * i) / (n - 1.)));
}

void FFT::fill(float *input)
{
    for (uint16_t i = 0; i < n; ++i)
    {
        if (i < (n / 2 + 1))
        {
            data[i] = input[i];
            window(data[i], i);
        }
        else
        {
            data[i] = 0;
        }
    }
}

void FFT::compute()
{
    fftw_execute(plan);
}

void FFT::magspec(vector<float> &buffer)
{
    for (uint16_t i = 0; i < (n / 2 + 1); ++i)
    {
        buffer[i] = std::abs(spectrum[i]);
    }
}
