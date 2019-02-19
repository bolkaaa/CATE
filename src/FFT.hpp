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
#include <iostream>

#include <fftw3.h>

using std::vector;
using std::complex;

template <class T>
class FFT
{
public:
    FFT(uint16_t sz);

    ~FFT();

    int show()
    {
        return data.size();
    }

    /* Fill data buffer with sample values from referenced buffer. */
    void fill(const vector<T> &buffer);

    /* Compute Discrete Fourier Transform of input buffer. */
    void compute();

    /* Calculate magnitude spectrum and pass to reference buffer. */
    void magspec(T *buffer);

private:
    uint16_t sz;
    vector<complex<T> > data;
    vector<complex<T> > spectrum;
    fftw_plan plan;
};

template <class T>
void FFT<T>::fill(const vector<T> &buffer)
{
    auto to_complex = [](T x) { return complex<T>(x, x); };
    std::transform(buffer.begin(), buffer.end(), data.begin(), to_complex);
}

template <class T>
void FFT<T>::compute()
{
    fftw_execute(plan);
}

template <class T>
FFT<T>::FFT(uint16_t sz)
    : sz(sz),
      data(vector<complex<T> >(sz)),
      spectrum(vector<complex<T> >(sz)),
      plan(fftw_plan_dft_1d(sz,
                            reinterpret_cast<fftw_complex*>(&data[0]),
                            reinterpret_cast<fftw_complex*>(&spectrum[0]),
                            FFTW_FORWARD, FFTW_ESTIMATE))
{
}

template <class T>
FFT<T>::~FFT()
{
    fftw_destroy_plan(plan);
}

#endif
