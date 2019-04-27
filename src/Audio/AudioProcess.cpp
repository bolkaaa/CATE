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
#include "../Analysis/FFT.hpp"
#include "../Synthesis/Scheduler.hpp"

#include <cstdlib>
#include <cmath>
#include <iostream>

namespace CATE {

AudioProcess::AudioProcess(Corpus &db, PointCloud &point_cloud, KdTree &kd_tree)
        : AudioEngine(48000.0f, 256, 2, 2),
          db(db),
          point_cloud(point_cloud),
          kd_tree(kd_tree),
          bin_size(1024),
          fft(FFT(bin_size, frames_per_buffer)),
          magspec(vector<float>(bin_size / 2 + 1)),
          feature(bin_size),
          return_indices(vector<size_t>(num_search_results)),
          distances(vector<float>(num_search_results)),
          markers(vector<int>(num_search_results)),
          filenames(vector<string>(num_search_results)),
          granulator(db.get_files(), sample_rate),
          gain_control(0.5),
          ready(false),
          audio_recorder(sample_rate),
          recording(false)
{
}

int AudioProcess::processing_callback(const void *input_buffer,
                                      void *output_buffer,
                                      unsigned long frames_per_buffer,
                                      const PaStreamCallbackTimeInfo *time_info,
                                      PaStreamCallbackFlags status_flags)
{
    static_cast<void>(status_flags);
    static_cast<void>(time_info);
    auto *input = const_cast<float *>(static_cast<const float *>(input_buffer));
    auto *output = static_cast<float *>(output_buffer);
    auto i = 0;
    float squared_input_sum = 0.0f;

    fft.fill(input);
    fft.compute();
    fft.get_magspec(magspec);
    float centroid = feature.centroid(magspec);
    float flatness = feature.flatness(magspec);
    float kurtosis = feature.kurtosis(magspec);

    /* KNN search. */
    const float search_points[3] = {
            centroid,
            flatness,
            kurtosis
    };

    kd_tree.knnSearch(&search_points[0], num_search_results, &return_indices[0], &distances[0]);

    for (i = 0; i < num_search_results; ++i)
    {
        markers[i] = point_cloud.points[return_indices[i]].marker;
        filenames[i] = point_cloud.points[return_indices[i]].file_path;
    }

    /* Main audio output block. */
    for (i = 0; i < frames_per_buffer; ++i)
    {
        squared_input_sum += std::pow(input[i], 2);

        float out = (gain_control) * granulator.synthesize(markers[0], filenames[0]);

        *output++ = out; // Left Channel
        *output++ = out; // Right Channel

        if (recording)
        {
            audio_recorder.write(out);
        }
    }

    input_rms = std::sqrt(squared_input_sum / frames_per_buffer);

    return paContinue;
}

void AudioProcess::start_recording()
{
    recording = true;
}

void AudioProcess::stop_recording()
{
    recording = false;
}

void AudioProcess::set_amplitude(float new_amplitude)
{
    gain_control = new_amplitude;
}

void AudioProcess::set_grain_attack(float new_grain_attack)
{
    granulator.set_grain_attack(new_grain_attack);
}

void AudioProcess::set_grain_release(float new_grain_release)
{
    granulator.set_grain_release(new_grain_release);
}

void AudioProcess::set_grain_density(int new_grain_density)
{
    granulator.set_grain_density(new_grain_density);
}

void AudioProcess::set_grain_width(int new_grain_width)
{
    granulator.set_grain_width(new_grain_width);
}

void AudioProcess::set_grain_size(int new_grain_size)
{
    granulator.set_grain_size(new_grain_size);
}

void AudioProcess::reload_granulator()
{
    granulator.load_files(db);
}

void AudioProcess::save_recording(const string &output_path)
{
    audio_recorder.save(output_path, output_channels, sample_rate);
}



} // CATE
