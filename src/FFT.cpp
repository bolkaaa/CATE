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

#include <cstdio>
#include <complex>
#include <vector>

#include <fftw3.h>

#include "FFT.hpp"

using std::vector;
using std::complex;
FFT::FFT(uint16_t bin_size)
    : bin_size(bin_size),
      data(vector<double>(bin_size)),
      spectrum(vector<complex<double> >(bin_size)),
      magspec(vector<float>(output_bins)),
      plan(fftw_plan_dft_r2c_1d(bin_size,
                                reinterpret_cast<double*>(&data[0]),
                                reinterpret_cast<fftw_complex*>(&spectrum[0]),
                                FFTW_ESTIMATE))
{
}

float FFT::window(uint16_t i)
{
    return (1./2.) * (1. - std::cos((2. * M_PI * i) / (bin_size - 1.)));
}

void FFT::fill(const vector<float> &input)
{
    for (auto i = 0; i < bin_size; ++i)
    {
        data[i] = input[i] * window(i);
    }
}

void FFT::compute()
{
    fftw_execute(plan);
    compute_magspec();
}

void FFT::get_magspec(vector<float> &output)
{
    for (auto i = 0; i < output_bins; ++i)
    {
        output[i] = magspec[i];
    }
}

void FFT::compute_magspec()
{
    for (auto i = 0; i < output_bins; ++i)
    {
        magspec[i] = std::abs(spectrum[i]);
    }
}

