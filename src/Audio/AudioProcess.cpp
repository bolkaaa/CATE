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

AudioProcess::AudioProcess(AudioSettings &audio_settings, Corpus &db, PointCloud &point_cloud, KdTree &kd_tree)
        : audio_settings(audio_settings),
          AudioEngine(audio_settings),
          db(db),
          point_cloud(point_cloud),
          kd_tree(kd_tree),
          fft(FFT(audio_settings)),
          magspec(vector<float>(audio_settings.get_bin_size() / 2 + 1)),
          feature(audio_settings.get_bin_size()),
          return_indices(vector<size_t>(num_search_results)),
          distances(vector<float>(num_search_results)),
          granulator(db.get_files(), audio_settings.get_sample_rate()),
          amplitude(0.5),
          ready(false),
          audio_recorder(audio_settings.get_sample_rate()),
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
    float input_sum = 0.0f;
    float rms = 0.0f;

    select_unit(input);

    /* Main audio output block. */
    for (i = 0; i < frames_per_buffer; ++i)
    {
        input_sum += std::pow(*input++, 2);

        float out = amplitude * granulator.synthesize(current_marker, current_file_path);

        *output++ = out; // L
        *output++ = out; // R

        if (recording)
        {
            audio_recorder.write(out);
        }
    }

    rms = std::sqrt(input_sum / frames_per_buffer);

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
    amplitude = new_amplitude;
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
    audio_recorder.save(output_path, audio_settings.get_num_output_channels(), audio_settings.get_sample_rate());
}

void AudioProcess::select_unit(float *input)
{
    fft.fill(input);
    fft.compute();
    fft.get_magspec(magspec);

    const float search_points[3] = {
            feature.centroid(magspec),
            feature.flatness(magspec),
            feature.kurtosis(magspec)
    };

    kd_tree.knnSearch(&search_points[0],
                      num_search_results,
                      &return_indices[0],
                      &distances[0]);

    current_marker = point_cloud.points[return_indices[0]].marker;
    current_file_path = point_cloud.points[return_indices[0]].file_path;
}

} // CATE
