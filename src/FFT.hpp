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

#include <cstdlib>
#include <cmath>
#include <complex>

#include <fftw3.h>

#include "AudioBuffer.hpp"

template <class T>
class FFT
{
public:
    FFT(uint16_t sz);

    void fill(T *input);

    /* Compute Discrete Fourier Transform of input data. */
    void compute();

    /* Calculate magnitude spectrum and pass to output buffer. */
    void magspec(vector<T> &buffer);

private:
    uint16_t n;
    double *data;
    fftw_complex *spectrum;
    fftw_plan plan;
    enum {REAL, IMAG};
};

template <class T>
FFT<T>::FFT(uint16_t n)
    : n(n),
      data(static_cast<double*> (fftw_malloc(sizeof(double) * n))),
      spectrum(static_cast<fftw_complex*> (fftw_malloc(sizeof(fftw_complex) * (n / 2 + 1)))),
      plan(fftw_plan_dft_r2c_1d(n,
                                data,
                                spectrum,
                                FFTW_ESTIMATE))
{
}

template <class T>
void FFT<T>::fill(T *input)
{
    for (uint16_t i = 0; i < n; ++i)
    {
        if (i < (n/2+1))
        {
            data[i] = input[i] * (1./2) * (1. - std::cos((2. * M_PI * i) / (n - 1.)));
        }
        else
        {
            data[i] = 0;
        }
    }
}

template <class T>
void FFT<T>::compute()
{
    fftw_execute(plan);
}

template <class T>
void FFT<T>::magspec(vector<T> &buffer)
{
    for (uint16_t i = 0; i < buffer.size(); ++i)
    {
        T mag = (spectrum[i][REAL] * spectrum[i][REAL]) + (spectrum[i][IMAG] * spectrum[i][IMAG]);
        mag = std::sqrt(mag);
        buffer[i] = mag;
    }
}

#endif
