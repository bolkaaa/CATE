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

AudioProcess::AudioProcess(AudioSettings *audio_settings, Corpus *corpus,
                           PointCloud *point_cloud, KdTree &kd_tree)
        : audio_settings(audio_settings),
          AudioEngine(audio_settings),
          corpus(corpus),
          point_cloud(point_cloud),
          kd_tree(kd_tree),
          fft(audio_settings),
          granulator(audio_settings),
          ring_buffer(new RingBuffer(ring_buffer_size)),
          return_indices(num_search_results),
          distances(num_search_results),
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
    auto *input = static_cast<const float *>(input_buffer);
    auto *output = static_cast<float *>(output_buffer);

    auto unit = select_unit(input, buffer_size);

    /* Main audio output block. */
    for (unsigned long i = 0; i < buffer_size; ++i)
    {
        const float out = granulator.synthesize(unit);

        *output++ = out; // L
        *output++ = out; // R

        /* While recording, current output sample is continuously written to ring buffer. */
        if (recording)
        {
            ring_buffer->push(out);
            emit send_record_data(ring_buffer);
        }
    }

    return paContinue;
}

void AudioProcess::reload_granulator()
{
    granulator.load_files(corpus);
    granulator.rebuild_grain_pool();
}

Unit AudioProcess::select_unit(const float *input, int n)
{
    auto unit = Unit();

    compute_magspec(input, n);

    const float search_points[FeatureMap::num_features] = {
            spectral_centroid(magspec),
            spectral_rolloff(magspec),
            spectral_flatness(magspec),
    };

    kd_tree.knnSearch(&search_points[0],
                      num_search_results,
                      &return_indices[0],
                      &distances[0]);

    auto point_cloud_index = return_indices[0];
    unit.marker = point_cloud->get_marker(point_cloud_index);
    unit.file_path = point_cloud->get_file_path(point_cloud_index);

    return unit;
}

void AudioProcess::compute_magspec(const float *input, int n)
{
    fft.fill(input, n);
    fft.compute_spectrum();
    fft.compute_magspec();
    magspec = fft.get_magspec();
}

void AudioProcess::analyse_directory(const Path &directory_path)
{
    corpus->add_directory(directory_path);
    corpus->sliding_window_analysis();
}

void AudioProcess::rebuild_data_points()
{
    corpus->rebuild_point_cloud();
    kd_tree.buildIndex();
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
