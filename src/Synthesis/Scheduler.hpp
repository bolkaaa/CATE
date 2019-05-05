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
#include <array>

#include "src/Audio/AudioSettings.hpp"
#include "src/Audio/AudioFile.hpp"
#include "Grain.hpp"
#include "EnvelopeParams.hpp"
#include "Rand.hpp"
#include "GrainParams.hpp"

using std::vector;
using std::array;

namespace CATE {

/* Handles activation of grains, based upon onset times. It handles grain allocation and mixing of grains to form a
 * single output. */

typedef vector<Grain> GrainPool;

class Scheduler
{
public:
    Scheduler(AudioSettings *audio_settings, GrainParams *grain_params,
              EnvelopeParams *envelope_params);

    /* Load audio file map into scheduler. */
    void load_files(const map<string, AudioFile> &new_files);

    /* Calculate grain activations. */
    float schedule(int marker, const string &filename);

    /* Rebuild grain pool according to size changes. */
    void rebuild_grain_pool();

private:
    /* Mix all currently active grains to a single output. */
    float synthesize_grains();

    /* Create a new grain object at next index of grain pool. */
    void create_grain(int marker, const string &file_name);

    /* Stochastically generate next inter-onset value. */
    int get_next_inter_onset();

    /* Fill buffer from filename and file position. */
    void fill_buffer(int marker, const string &file_name);

    AudioSettings *audio_settings;
    GrainParams *grain_params;
    EnvelopeParams *envelope_params;
    GrainPool grains;
    map<string, AudioFile> files;
    AudioBuffer buffer;
    int next_onset;
    Rand<float> rand;
};

} // CATE

#endif
