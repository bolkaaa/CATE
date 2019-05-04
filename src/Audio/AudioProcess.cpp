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

AudioProcess::AudioProcess(const unique_ptr<AudioSettings> &audio_settings, const unique_ptr<Corpus> &db,
                           const unique_ptr<PointCloud> &point_cloud, const KdTree &kd_tree)
        : audio_settings(audio_settings.get()),
          AudioEngine(audio_settings.get()),
          db(db.get()),
          point_cloud(point_cloud.get()),
          kd_tree(kd_tree),
          fft(audio_settings.get()),
          grain_params(new GrainParams),
          env_params(new EnvelopeParams(grain_params->get_grain_size())),
          granulator(audio_settings.get(), grain_params, env_params),
          return_indices(num_search_results),
          distances(num_search_results),
          ready(false),
          audio_recorder(audio_settings->get_sample_rate()),
          recording(false)
{
}

AudioProcess::~AudioProcess()
{
    delete grain_params;
    delete env_params;
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

    Unit unit = select_unit(input, grain_params->get_grain_size());

    /* Main audio output block. */
    for (unsigned long i = 0; i < buffer_size; ++i)
    {
        const float out = granulator.synthesize(unit);

        *output++ = out; // L
        *output++ = out; // R

        if (recording)
        {
            audio_recorder.write(out);
        }
    }

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

void AudioProcess::set_grain_sustain(float new_grain_sustain)
{
    env_params->set_sustain(new_grain_sustain);
}

void AudioProcess::set_grain_attack(float new_grain_attack)
{
    env_params->set_attack(new_grain_attack);
}

void AudioProcess::set_grain_release(float new_grain_release)
{
    env_params->set_release(new_grain_release);
}

void AudioProcess::set_grain_density(int new_grain_density)
{
    grain_params->set_grain_density(new_grain_density);
}

void AudioProcess::set_grain_size(int new_grain_size)
{
    grain_params->set_grain_size(new_grain_size);
}

void AudioProcess::reload_granulator()
{
    granulator.load_files(*db);
}

void AudioProcess::save_recording(const string &output_path)
{
    audio_recorder.save(output_path, get_num_output_channels(), audio_settings->get_sample_rate());
}

Unit AudioProcess::select_unit(const float *input, int n)
{
    Unit unit;

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

    int point_cloud_index = return_indices[0];
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


} // CATE
