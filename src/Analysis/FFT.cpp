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
using CATE::AudioBuffer;

namespace CATE {

FFT::FFT(int bin_size, int frames_per_buffer)
        : bin_size(bin_size),
          output_size(bin_size / 2 + 1),
          magspec(vector<float>(output_size)),
          frames_per_buffer(frames_per_buffer),
          data(vector<double>(bin_size)),
          spectrum(vector<complex<double> >(output_size)),
          plan(fftw_plan_dft_r2c_1d(bin_size,
                                    reinterpret_cast<double *>(&data[0]),
                                    reinterpret_cast<fftw_complex *>(&spectrum[0]),
                                    FFTW_ESTIMATE))
{
}

float FFT::window(int i)
{
    auto hanning = static_cast<float>((1. / 2.) * (1. - std::cos((2. * M_PI * i) / (bin_size - 1.))));

    return hanning;
}

void FFT::fill(float *input)
{
    /* Fill data array with input multiplied by windowing function. */
    for (auto i = 0; i < frames_per_buffer; ++i)
    {
        data[i] = input[i] * window(i);
    }

    /* Pad range from <frames_per_buffer> to <bin_size> with zeroes. */
    for (auto i = frames_per_buffer; i < bin_size; ++i)
    {
        data[i] = 0.0;
    }
}

void FFT::compute()
{
    fftw_execute(plan);
    compute_magspec();
}

void FFT::get_magspec(vector<float> &output)
{
    for (auto i = 0; i < output_size; ++i)
    {
        output[i] = magspec[i];
    }
}

void FFT::compute_magspec()
{
    for (auto i = 0; i < output_size; ++i)
    {
        magspec[i] = std::abs(spectrum[i]);
    }
}

} // CATE
