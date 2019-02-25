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

#include <complex>
#include <vector>
#include <iostream>

#include <fftw3.h>

using std::vector;
using std::complex;

/*
  The FFT class wraps the fftw library, providing functionality for calculating
  Discrete Fourier Fransforms of input data.
 */

class FFT
{
public:
    FFT(uint16_t bin_size);

    /* Fill input data from outside source. */
    void fill(const vector<float> &input);

    /* Compute DFT and magnitude spectrum of input data. */
    void compute();

    /* Calculate magnitude spectrum and pass to output buffer. */
    void get_magspec(vector<float> &output);

private:
    /* Calculate Hanning window function. */
    float window(uint16_t i);

    /* Compute magnitude spectrum from complex spectrum. */
    void compute_magspec();

    const uint16_t bin_size;
    const uint16_t output_bins = bin_size / 2 + 1;
    vector<double> data;
    vector<complex<double> > spectrum;
    vector<float> magspec;
    fftw_plan plan;
};

#endif
