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

AudioProcess::AudioProcess(float sample_rate, int frames_per_buffer, int input_channels, int output_channels,
                           int fft_bin_size, Database &db, PointCloud &point_cloud, KdTree &kd_tree)
        : AudioEngine(sample_rate, frames_per_buffer, input_channels, output_channels),
          fft_bin_size(fft_bin_size),
          fft(FFT(fft_bin_size, frames_per_buffer)),
          magspec(vector<float>(fft_bin_size / 2 + 1)),
          db(db),
          point_cloud(point_cloud),
          feature(fft_bin_size),
          kd_tree(kd_tree),
          return_indices(vector<size_t>(search_results)),
          distances(vector<float>(search_results)),
          granulator(db.get_files(), sample_rate),
          gain_control(0.5),
          input_buffer(AudioBuffer(frames_per_buffer)),
          ready(false),
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

    /* KNN search. */
    const float search_points[3] = {
            centroid,
            flatness
    };

    kd_tree.knnSearch(&search_points[0], search_results, &return_indices[0], &distances[0]);
    int marker = point_cloud.points[return_indices[0]].marker;
    string file_path = point_cloud.points[return_indices[0]].file_path;

    /* Main audio output block. */
    for (i = 0; i < frames_per_buffer; ++i)
    {
        squared_input_sum += std::pow(input[i], 2);

        float out = (input_rms * gain_control) * granulator.synthesize(marker, file_path);

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

void AudioProcess::reload_granulator()
{
    granulator.load_files(db);
}

void AudioProcess::save_recording(string output_path)
{
    audio_recorder.save(output_path, output_channels, sample_rate);
}


} // CATE
