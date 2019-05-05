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

namespace CATE {

FFT::FFT(AudioSettings *audio_settings)
        : audio_settings(audio_settings),
          output_size(audio_settings->get_bin_size().value / 2 + 1),
          data(audio_settings->get_bin_size().value),
          spectrum(output_size),
          magspec(output_size),
          plan(fftw_plan_dft_r2c_1d(audio_settings->get_bin_size().value,
                                    reinterpret_cast<double *>(&data[0]),
                                    reinterpret_cast<fftw_complex *>(&spectrum[0]),
                                    FFTW_ESTIMATE))
{
}

FFT::~FFT()
{
    fftw_destroy_plan(plan);
}

float FFT::window(int i, int n)
{
    auto hanning = static_cast<float>((1. / 2.) * (1. - std::cos((2. * M_PI * i) / (n - 1.))));
    return hanning;
}

void FFT::fill(const float *input, int n)
{
    /* Fill data array with input multiplied by windowing function. */
    for (auto i = 0; i < n; ++i)
    {
        data[i] = input[i] * window(i, n);
    }

    /* Pad range from <n> to <bin_size> with zeroes. */
    for (auto i = n; i < audio_settings->get_bin_size().value; ++i)
    {
        data[i] = 0.0;
    }
}

void FFT::compute_spectrum()
{
    fftw_execute(plan);
}

void FFT::compute_magspec()
{
    for (auto i = 0; i < output_size; ++i)
    {
        magspec[i] = static_cast<float>(std::abs(spectrum[i]));
    }
}

} // CATE
