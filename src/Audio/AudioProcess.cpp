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
          fft(FFT(fft_bin_size, frames_per_buffer)),
          feature(fft_bin_size),
          magspec(vector<float>(fft_bin_size / 2 + 1)),
          db(db),
          point_cloud(point_cloud),
          kd_tree(kd_tree),
          return_indices(vector<size_t>(search_results)),
          distances(vector<float>(search_results)),
          granulator(db.get_files(), sample_rate),
          gain_control(0.5),
          max_recording_length(30),
          input_buffer(vector<float>(frames_per_buffer)),
          recording_data(AudioBuffer(max_recording_length * sample_rate)),
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

    /* FFT operations. */
    fft.fill(input);
    fft.compute();
    fft.get_magspec(magspec);
    centroid = feature.centroid(magspec);
    flatness = feature.flatness(magspec);
    kurtosis = feature.kurtosis(magspec);

    /* KNN search. */
    const float search_points[2] = {centroid, flatness};
    kd_tree.knnSearch(&search_points[0], search_results, &return_indices[0], &distances[0]);
    int marker = point_cloud.points[return_indices[0]].marker;
    string file_path = point_cloud.points[return_indices[0]].file_path;

    float squared_input_sum = 0.0f;

    /* Main audio output block. */
    for (i = 0; i < frames_per_buffer; ++i)
    {
        squared_input_sum += std::pow(input[i], 2);

        float out = (gain_control * input_rms) * granulator.synthesize(marker, file_path);

        *output++ = out; // Left Channel
        *output++ = out; // Right Channel

        if (recording)
        {
            recording_data[i] = output[i];
        }
    }

    input_rms = std::sqrt(squared_input_sum / frames_per_buffer);

    return paContinue;
}

void AudioProcess::start_recording()
{
    std::cout << "Starting recording. (test)\n";

    recording = true;
}

void AudioProcess::stop_recording()
{
    std::cout << "Stopping recording. (test)\n";

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

} // CATE
