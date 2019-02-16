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

#ifndef SYNTH_HPP
#define SYNTH_HPP

#include <cmath>

#include "AudioBuffer.hpp"
#include "Database.hpp"
#include "RingBuffer.hpp"
#include "FFT.hpp"

#include "portaudio.h"

template <class T>
class Synth
{
public:
    /* Instantiate by setting size in samples for audio processing buffers. */
    Synth<T>(uint16_t buffer_size);

    /* Main audio processing function to be called from outside class. */
    int process(const void *input, void *output,
                unsigned long frames_per_buffer,
                const PaStreamCallbackTimeInfo *time_info,
                PaStreamCallbackFlags status_flags);

private:
    /* Provide data from audio input to ring buffer and pop to processing buffer. */
    void prepare_buffers(T **in);

    /* Take DFT of signal, TODO: add sliding window */
    void analyse();

    uint16_t buffer_size;
    RingBuffer<T> input_buffer;
    T *process_buffer;
    FFT<T> fft;
};

template <class T>
Synth<T>::Synth(uint16_t buffer_size)
    : buffer_size(buffer_size), fft(buffer_size), input_buffer(buffer_size, buffer_size/4),
      process_buffer(new T[buffer_size])
{
}

template <class T>
void Synth<T>::prepare_buffers(T **in)
{
    /* Get data from input into ring buffer. */
    for (uint16_t i = 0; i < buffer_size; ++i)
    {
        input_buffer.push(in[0][i]);
    }

    /* Pop ring buffer data into process buffer. */
    for (uint16_t i = 0; i < buffer_size; ++i)
    {
        input_buffer.pop(process_buffer[i]);
    }

    fft.fill(process_buffer);
}

template <class T>
void Synth<T>::analyse()
{
    fft.compute();
}

template <class T>
int Synth<T>::process(const void *input, void *output,
                      unsigned long frames_per_buffer,
                      const PaStreamCallbackTimeInfo *time_info,
                      PaStreamCallbackFlags status_flags)
{
    (void) status_flags;
    (void) time_info;
    T **out = (T**) (output);
    T **in = (T**) (input);

    prepare_buffers(in);

    analyse();

    /* Main processing block. */
    for (uint32_t i = 0; i < frames_per_buffer; i++)
    {
        out[0][i] = process_buffer[i];
        out[1][i] = process_buffer[i];
    }

    return paContinue;
}

#endif
