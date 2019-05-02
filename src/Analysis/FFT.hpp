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

#ifndef FFT_HPP
#define FFT_HPP

#include <vector>
#include <complex>
#include <iostream>

#include <fftw3.h>

#include "src/Audio/AudioBuffer.hpp"
#include "src/Audio/AudioSettings.hpp"

using std::vector;
using std::complex;

/* The FFT class wraps the fftw library, providing functionality for calculating
 * Discrete Fourier Transforms of input data. It uses the C++ complex class to
 * represent complex numbers rather than fftw_complex, allowing for some
 * conveniences such as using std::abs to calculate the magnitude spectrum. */

namespace CATE {

class FFT
{
public:
    FFT(const AudioSettings &audio_settings);

    ~FFT();

    /* Fill input data array. */
    void fill(const float *input);

    /* Compute complex spectrum of input data. */
    void compute_spectrum();

    /* Compute magnitude spectrum from complex spectrum. */
    void compute_magspec();

    /* Get magnitude spectrum vector. */
    inline vector<float> get_magspec() { return magspec; };

    /* Calculate Hanning window function for a given index and size n. */
    float window(int i, int n);

private:
    const AudioSettings &audio_settings;
    int output_size;
    vector<double> data;
    vector<complex<double> > spectrum;
    vector<float> magspec;
    fftw_plan plan;
};

} // CATE

#endif