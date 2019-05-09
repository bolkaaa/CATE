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
                     Param<float> *grain_release, Param<float> *grain_density, Param<float> *grain_size,
                     FixedParam<int> *max_grains)
        : audio_settings(audio_settings),
          grain_attack(grain_attack),
          grain_sustain(grain_sustain),
          grain_release(grain_release),
          grain_density(grain_density),
          grain_size(grain_size),
          max_grains(max_grains),
          next_onset(0),
          rand(0.01f, 1.0f)
{
}

void Scheduler::activate_next_grain()
{
    for (auto i : indices)
    {
        if (!grain_pool[i].is_active())
        {
            auto grain_size_samples = static_cast<int>((grain_size->value * audio_settings->get_sample_rate().value) / 1000.0f);
            grain_pool[i].activate(grain_size_samples,
                                   grain_attack->value,
                                   grain_sustain->value,
                                   grain_release->value);

            return;
        }
    }
}

float Scheduler::schedule(int new_grain_index)
{
    if (next_onset == 0)
    {
        activate_next_grain();
        next_onset += get_next_inter_onset();
    }

    --next_onset;

    auto output = synthesize_grains();

    return output;
}

float Scheduler::synthesize_grains()
{
    auto out = 0.0f;

    for (auto i : indices)
    {
        if (grain_pool[i].is_active())
        {
            out += grain_pool[i].synthesize();
        }
    }

    return out;
}

int Scheduler::get_next_inter_onset()
{
    auto random_value = rand.get();
    auto grains_per_second = audio_settings->get_sample_rate().value / grain_density->value;
    auto inter_onset = 1 + static_cast<int>(grains_per_second * random_value);
    return inter_onset;
}

void Scheduler::rebuild_grain_pool(GrainPool grain_pool)
{
    Scheduler::grain_pool = grain_pool;

    /* Initialise grains to random indices. */
    for (auto i = 0; i < max_grains->value; ++i)
    {
        auto r = rand.get();
        auto n = static_cast<float>(grain_pool.size());
        auto index = r * n;
        indices.emplace_back(index);
    }
}

} // CATE
