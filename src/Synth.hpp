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

#include <vector>
#include <cstdio>

#include "Database.hpp"
#include "AudioBuffer.hpp"
#include "RingBuffer.hpp"
#include "FFT.hpp"

#include "portaudio.h"

using std::vector;

template <class T>
class Synth
{
public:
    /* Instantiate by setting size in samples for audio processing buffers. */
    Synth<T>(uint16_t buffer_size, uint8_t num_channels);

    /* Main audio processing function to be called from outside class. */
    int process(const void *input, void *output,
                unsigned long frames_per_buffer,
                const PaStreamCallbackTimeInfo *time_info,
                PaStreamCallbackFlags status_flags);

private:
    /* Provide audio input to auxillary buffers. */
    void prepare_buffers(T **in);

    uint16_t buffer_size;
    uint8_t num_channels;
    vector<vector<T> > input_buffer;
    vector<vector<T> > spectrum_buffer;
    vector<RingBuffer<T> > ring_buffer;
    vector<FFT<T> > fft_buffer;
};

template <class T>
Synth<T>::Synth(uint16_t buffer_size, uint8_t num_channels)
    : buffer_size(buffer_size), num_channels(num_channels),
      input_buffer(num_channels, vector<T>(buffer_size)),
      spectrum_buffer(num_channels, vector<T>(buffer_size)),
      ring_buffer(num_channels, RingBuffer<T>(buffer_size)),
      fft_buffer(num_channels, FFT<T>(buffer_size))
{
}

template <class T>
void Synth<T>::prepare_buffers(T **in)
{
    for (uint8_t chan = 0; chan < num_channels; ++chan)
    {
        for (uint16_t i = 0; i < buffer_size; ++i)
        {
            ring_buffer[chan].push(in[chan][i]);
        }
    }

    for (uint8_t chan = 0; chan < num_channels; ++chan)
    {
        for (uint16_t i = 0; i < buffer_size; ++i)
        {
            ring_buffer[chan].pop(input_buffer[chan][i]);
        }
    }


    for (uint8_t chan = 0; chan < num_channels; ++chan)
    {

        fft_buffer[chan].fill(&input_buffer[chan][0]);
        fft_buffer[chan].compute();
        fft_buffer[chan].magspec(spectrum_buffer[chan]);
        std::cout << spectrum_buffer[chan][512] << "\n";
    }
}

template <class T>
int Synth<T>::process(const void *input, void *output,
                      unsigned long frames_per_buffer,
                      const PaStreamCallbackTimeInfo *time_info,
                      PaStreamCallbackFlags status_flags)
{
    (void) status_flags;
    (void) time_info;
    T **in = (T**) (input);
    T **out = (T**) (output);

    prepare_buffers(in);

    /* Main audio output block. */
    for (uint32_t i = 0; i < frames_per_buffer; i++)
    {
        for (uint8_t chan = 0; chan < num_channels; ++chan)
        {
            out[chan][i] = input_buffer[chan][i];
        }
    }

    return paContinue;
}

#endif
