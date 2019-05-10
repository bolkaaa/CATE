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
#include "src/Corpus/Corpus.hpp"

namespace CATE {

AudioProcess::AudioProcess(AudioSettings *audio_settings, Corpus *corpus)
        : audio_settings(audio_settings),
          AudioEngine(audio_settings),
          corpus(corpus),
          granulator(audio_settings),
          input_ring_buffer(new RingBuffer<float>(ring_buffer_size)),
          output_ring_buffer(new RingBuffer<float>(ring_buffer_size)),
          corpus_index_input_queue(new RingBuffer<CorpusIndex>(ring_buffer_size)),
          corpus_index_output_queue(new RingBuffer<CorpusIndex>(ring_buffer_size)),
          recording(false),
          audio_loaded(false)
{
}

AudioProcess::~AudioProcess()
{
    delete corpus_index_output_queue;
    delete corpus_index_input_queue;
    delete output_ring_buffer;
    delete input_ring_buffer;
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

        /* TODO: optimise analysis code. */
        /* Send input to ring buffer. */
        input_ring_buffer->push(in);

        if (!input_ring_buffer->space_available())
        {
            emit send_input_data(input_ring_buffer);
        }

        /* Get next analysis result. */
        search_results_received(corpus_index_input_queue);

        /* Synthesize output from granulation engine. */
        auto out = granulator.synthesize(corpus_index_output_queue);

        *output++ = out; // Left Channel
        *output++ = out; // Right Channel

        /* While recording, current output sample is continuously written to ring buffer. */
        if (recording)
        {
            output_ring_buffer->push(out);

            if (!output_ring_buffer->space_available())
            {
                emit send_output_data(output_ring_buffer);
            }
        }
    }

    return paContinue;
}

void AudioProcess::load_audio(const AudioFrameMap &audio_frame_map)
{
    granulator.calculate_grain_pool(audio_frame_map);
}

void AudioProcess::search_results_received(RingBuffer<CorpusIndex> *corpus_index_input_queue)
{
    if (corpus_index_input_queue->samples_available())
    {
        auto index = CorpusIndex();
        corpus_index_input_queue->pop(index);
        corpus_index_output_queue->push(index);
    }
}

} // CATE
