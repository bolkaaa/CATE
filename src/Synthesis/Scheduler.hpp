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

#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <vector>
#include <queue>

#include "src/Audio/AudioSettings.hpp"
#include "src/Audio/AudioFile.hpp"
#include "src/Audio/AnalysisWorker.hpp"
#include "Grain.hpp"
#include "Rand.hpp"

namespace CATE {

typedef vector<Grain> GrainPool;

/* Handles activation of grains, based upon onset times, as well as mixing of grains to form a
 * single output. */
class Scheduler
{
public:
    Scheduler(AudioSettings *audio_settings, Param<float> *grain_attack, Param<float> *grain_sustain,
              Param<float> *grain_release, Param<float> *grain_density, Param<float> *grain_size,
              FixedParam<int> *max_grains);

    /* Calculate grain activations. */
    float schedule(int new_grain_index);

    /* Rebuild grain pool according to size changes. */
    void rebuild_grain_pool(GrainPool grain_pool);

private:
    /* Mix all currently active grains to a single output. */
    float synthesize_grains();

    /* Iterate over grain indices; if inactive grain is found, activate it and exit the function. */
    void activate_next_grain();

    /* Stochastically generate next inter-onset value according to grain density parameter. */
    int get_next_inter_onset();

    AudioSettings *audio_settings;
    Param<float> *grain_attack;
    Param<float> *grain_sustain;
    Param<float> *grain_release;
    Param<float> *grain_density;
    Param<float> *grain_size;
    FixedParam<int> *max_grains;
    AudioBuffer buffer = AudioBuffer(grain_size->max);
    vector<int> indices;
    GrainPool grain_pool;
    map<string, AudioFile> files;
    int next_onset;
    Rand<float> rand;
};

} // CATE

#endif
