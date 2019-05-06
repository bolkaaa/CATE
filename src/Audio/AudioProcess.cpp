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

#include <cstdlib>
#include <cmath>
#include <iostream>

#include "AudioProcess.hpp"
#include "src/Analysis/FFT.hpp"
#include "src/Synthesis/Scheduler.hpp"

namespace CATE {

AudioProcess::AudioProcess(AudioSettings *audio_settings, Corpus *corpus)
        : audio_settings(audio_settings),
          AudioEngine(audio_settings),
          corpus(corpus),
          granulator(audio_settings),
          input_ring_buffer(new RingBuffer(ring_buffer_size)),
          output_ring_buffer(new RingBuffer(ring_buffer_size)),
          recording(false)
{
}

int AudioProcess::processing_callback(const void *input_buffer,
                                      void *output_buffer,
                                      unsigned long buffer_size,
                                      const PaStreamCallbackTimeInfo *time_info,
                                      PaStreamCallbackFlags status_flags)
{
    static_cast<void>(status_flags);
    static_cast<void>(time_info);
    const auto *input = static_cast<const float *>(input_buffer);
    auto *output = static_cast<float *>(output_buffer);

    /* Main audio output block. */
    for (unsigned long i = 0; i < buffer_size; ++i)
    {
        const auto in = *input++;

        input_ring_buffer->push(in);

        emit send_input_data(input_ring_buffer);

        // const auto out = granulator.synthesize(unit);
        const auto out = 0.0f;

        *output++ = out; // L
        *output++ = out; // R

        /* While recording, current output sample is continuously written to ring buffer. */
        if (recording)
        {
            output_ring_buffer->push(out);
            emit send_output_data(output_ring_buffer);
        }
    }

    return paContinue;
}

void AudioProcess::reload_granulator()
{
    granulator.load_files(corpus);
    granulator.rebuild_grain_pool();
}

void AudioProcess::analyse_directory(const Path &directory_path)
{
    corpus->add_directory(directory_path);
    corpus->sliding_window_analysis();
}

void AudioProcess::rebuild_data_points()
{
    corpus->rebuild_point_cloud();
    corpus->rebuild_index();
    reload_granulator();
}

void AudioProcess::load_corpus(const Path &corpus_path)
{
    corpus->read_file(corpus_path);
    corpus->load_audio_from_db();
    granulator.load_files(corpus);
    rebuild_data_points();
}

} // CATE
