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

#include "src/Corpus/PathTree.hpp"
#include "src/Corpus/Corpus.hpp"
#include "Scheduler.hpp"

namespace CATE {

Scheduler::Scheduler(AudioSettings *audio_settings, Param<float> *grain_attack, Param<float> *grain_sustain,
                     Param<float> *grain_release, Param<float> *grain_density, Param<int> *grain_size,
                     FixedParam<int> *max_grains)
        : audio_settings(audio_settings),
          grain_attack(grain_attack),
          grain_sustain(grain_sustain),
          grain_release(grain_release),
          grain_density(grain_density),
          grain_size(grain_size),
          max_grains(max_grains),
          grain_pool(static_cast<std::size_t>(max_grains->value)),
          next_onset(0),
          rand(0.01f, 1.0f)
{
}

void Scheduler::activate_next_grain()
{
    for (auto &grain : grain_pool)
    {
        if (!grain.is_active())
        {
            grain.activate(grain_size->value);

            return;
        }
    }
}

float Scheduler::schedule()
{
    if (next_onset == 0)
    {
        activate_next_grain();
        next_onset += get_next_inter_onset();
    }

    --next_onset;

    return synthesize_grains();
}

float Scheduler::synthesize_grains()
{
    auto output = 0.0f;

    for (auto &grain : grain_pool)
    {
        if (grain.is_active())
        {
            output += grain.synthesize(grain_size->value,
                                       grain_attack->value,
                                       grain_sustain->value,
                                       grain_release->value);
        }
    }

    return output;
}

int Scheduler::get_next_inter_onset()
{
    auto random_value = rand.get();
    auto grains_per_second = audio_settings->get_sample_rate().value / grain_density->value;
    auto inter_onset = 1 + static_cast<int>(grains_per_second * random_value);
    return inter_onset;
}

void Scheduler::load_files(const AudioBufferMap &new_buffers)
{
    for (auto &grain : grain_pool)
    {
        for (auto i = 0; i < grain_size->max; ++i)
        {
            auto value = 0.0f; // set to audio file data.
            grain.fill(value, i);
        }
    }
}

void Scheduler::rebuild_grain_pool()
{
    grain_pool = GrainPool(static_cast<std::size_t>(max_grains->value));
}


} // CATE
