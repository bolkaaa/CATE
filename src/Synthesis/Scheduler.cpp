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

#include <random>
#include <cmath>

#include "Scheduler.hpp"

namespace CATE {

Scheduler::Scheduler(AudioSettings *audio_settings, GrainParams grain_params,
                     EnvelopeParams envelope_params)
        : audio_settings(audio_settings),
          grain_params(grain_params),
          env_params(envelope_params),
          buffer(512),
          grains(512),
          next_onset(0),
          rand(Rand<float>(0.0f, 1.0f))
{
}

void Scheduler::fill_buffer(int size, int marker, const string &file_name)
{
    for (int i = 0; i < size; ++i)
    {
        int file_pos = i + marker;

        if (file_pos < files[file_name].data.size())
        {
            buffer[i] = files[file_name].data[file_pos];
        }
        else
        {
            buffer[i] = 0.0f;
        }
    }
}

void Scheduler::create_grain(int marker, const string &file_name)
{
    for (auto &grain : grains)
    {
        if (!grain.is_active())
        {
            auto grain_size = grain_params.get_grain_size();
            fill_buffer(grain_size, marker, file_name);
            grain = Grain(buffer, grain_size, env_params);
            return;
        }
    }
}

float Scheduler::schedule(int marker, const string &filename)
{
    if (next_onset == 0)
    {
        create_grain(marker, filename);
        next_onset += get_next_inter_onset();
    }

    --next_onset;

    return synthesize_grains();
}

float Scheduler::synthesize_grains()
{
    float output = 0.0f;

    for (auto &grain : grains)
    {
        if (grain.is_active())
        {
            output += grain.synthesize();
        }
    }

    return output;
}

int Scheduler::get_next_inter_onset()
{
    float random_value = rand.get();
    auto grains_per_second = audio_settings->get_sample_rate() / grain_params.get_grain_density();
    auto inter_onset = static_cast<int>(1 + (grains_per_second * random_value));
    return inter_onset;
}

void Scheduler::load_files(const map<string, AudioFile> &files)
{
    Scheduler::files = files;
}

void Scheduler::rebuild_grain_pool()
{
    auto max_grains = grain_params.get_max_grains();
    grains = GrainPool(max_grains);
}


} // CATE
