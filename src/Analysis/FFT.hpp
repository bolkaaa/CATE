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

typedef vector<complex<double> > Spectrum;
typedef vector<float> Magspec;

class FFT
{
public:
    explicit FFT(AudioSettings *audio_settings);

    ~FFT();

    /* Fill input data array. */
    void fill(AudioBuffer input);

    /* Compute complex spectrum of input data. */
    void compute_spectrum();

    /* Compute magnitude spectrum from complex spectrum. */
    void compute_magspec();

    /* Calculate Hanning window function for a given index and size n. */
    float window(int i, int n);

    /* Return magnitude spectrum value at specified bin. */
    float get_magspec_bin(int i) { return magspec[i]; }

    /* Copy values from internal magspec object to external reference object. */
    void get_magspec(Magspec &output)
    {
        for (auto i = 0; i < output_size; ++i)
        {
            output[i] = magspec[i];
        }
    }

    Magspec get_magspec() const
    {
        return magspec;
    }

private:
    AudioSettings *audio_settings;
    int output_size;
    vector<double> data;
    Spectrum spectrum;
    Magspec magspec;
    fftw_plan plan;
};

} // CATE

#endif