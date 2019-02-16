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

#include <fftw3.h>

template <class T>
class FFT
{
public:
    FFT(uint16_t sz);

    ~FFT();

    /* Fill data buffer with sample values from referenced buffer. */
    void fill(T *buffer);

    /* Get number of samples in buffers. */
    uint16_t size() { return sz; }

    /* Compute Discrete Fourier Transform of input buffer. */
    void compute();

    /* Calculate magnitude spectrum and pass to reference buffer. */
    void magspec(T *buffer);

private:
    uint16_t sz;
    fftw_complex *data;
    fftw_complex *spectrum;
    fftw_plan plan;
    enum { REAL, IMAG };
};

template <class T>
void FFT<T>::fill(T *buffer)
{
    for (uint16_t i = 0; i < sz; ++i)
    {
        data[i][REAL] = buffer[i];
        data[i][IMAG] = data[i][REAL];
    }
}


template <class T>
void FFT<T>::compute()
{
    fftw_execute(plan);
}

template <class T>
FFT<T>::FFT(uint16_t sz)
    : sz(sz),
      data(static_cast<fftw_complex*> (fftw_malloc(sizeof(fftw_complex) * sz))),
      spectrum(static_cast<fftw_complex*> (fftw_malloc(sizeof(fftw_complex) * sz))),
      plan(fftw_plan_dft_1d(sz, data, spectrum, FFTW_FORWARD, FFTW_ESTIMATE))
{
}

template <class T>
FFT<T>::~FFT()
{
    fftw_destroy_plan(plan);
    fftw_free(data);
    fftw_free(spectrum);
}

template <class T>
void FFT<T>::magspec(T *buffer)
{
    for (uint16_t i = 0; i < sz; ++i)
    {
        T real_sq = spectrum[i][REAL] * spectrum[i][REAL];
        T imag_sq = spectrum[i][IMAG] * spectrum[i][IMAG];
        T sqrt = std::sqrt(real_sq + imag_sq);
        buffer[i] = sqrt;
    }
}

#endif
