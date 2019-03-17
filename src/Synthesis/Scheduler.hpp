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
#include <random>

#include "src/Audio/AudioFile.hpp"
#include "Grain.hpp"

using std::vector;
using std::random_device;
using std::mt19937;
using std::uniform_real_distribution;

namespace CATE {

/* Handles activation of grains, based upon onset times. It handles grain allocation and mixing of grains to form a
 * single output. */
class Scheduler
{
public:
    /* Calculate grain activations. */
    float schedule();

    Scheduler(vector<AudioFile> &files, float sample_rate);

    /* Mix all currently active grains to a single output. */
    float synthesize_grains();

private:
    /* Stochastically generate next inter-onset value, based on a density parameter. */
    int get_next_inter_onset();

    vector<AudioFile> files;
    vector<Grain> grains;
    random_device seed;
    mt19937 gen;
    uniform_real_distribution<float> dist;
    float grain_density;
    int max_grains;
    int max_onset;
    int next_onset;
    int grain_index;
    int sample_count;
    float sample_rate;
};

} // CATE

#endif
