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

#include "AudioProcess.hpp"
#include "Synth.hpp"
#include "FFT.hpp"

#include <cstdlib>
#include <cmath>
#include <iostream>

AudioProcess::AudioProcess(uint16_t sample_rate, uint16_t frames_per_buffer,
                           uint16_t fft_bin_size)
    : AudioEngine(sample_rate, frames_per_buffer)
{
    synth = new Synth(fft_bin_size, frames_per_buffer);
}

AudioProcess::~AudioProcess()
{
    delete synth;
}

int AudioProcess::processing_callback(const void *input_buffer,
                                      void *output_buffer,
                                      unsigned long frames_per_buffer,
                                      const PaStreamCallbackTimeInfo* time_info,
                                      PaStreamCallbackFlags status_flags)
{
    static_cast<void>(status_flags);
    static_cast<void>(time_info);
    float *input = const_cast<float*>(static_cast<const float*>(input_buffer));
    float *output = static_cast<float*>(output_buffer);
    unsigned long i = 0;

    /* Basic Input -> Output (watch for feedback) */
    for (i = 0; i < frames_per_buffer; ++i)
    {
        output[i] = input[i];
    }

    return paContinue;
}
