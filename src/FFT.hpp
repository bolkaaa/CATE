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

using std::vector;
using std::complex;

template <class T>
class FFT
{
public:
    FFT(uint16_t sz);

    ~FFT();

    /* Compute Discrete Fourier Transform of input data. */
    void compute();

    void fill(const vector<T> &buffer)
    {
        data = buffer;
    }

    /* Calculate magnitude spectrum and pass to output buffer. */
    void magspec(vector<T> &buffer);

private:
    vector<T> data;
    vector<complex<T> > spectrum;
    fftw_plan plan;
};

template <class T>
FFT<T>::FFT(uint16_t sz)
    : data(vector<T>(sz)),
      spectrum(vector<complex<T> >(sz)),
      plan(fftw_plan_dft_r2c_1d(sz,
                                reinterpret_cast<double*>(&data[0]),
                                reinterpret_cast<fftw_complex*>(&spectrum[0]),
                                FFTW_ESTIMATE))
{
}

template <class T>
FFT<T>::~FFT()
{
    fftw_destroy_plan(plan);
}

template <class T>
void FFT<T>::compute()
{
    fftw_execute(plan);
}

template <class T>
void FFT<T>::magspec(vector<T> &buffer)
{
    for (uint16_t i = 0; i < data.size(); ++i)
    {
        buffer[i] = std::abs(spectrum[i]);
    }
}

#endif
