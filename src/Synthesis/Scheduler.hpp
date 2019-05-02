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

#include "src/Audio/AudioFile.hpp"
#include "Grain.hpp"
#include "EnvelopeParams.hpp"
#include "Rand.hpp"

using std::vector;
using std::array;

namespace CATE {

/* Handles activation of grains, based upon onset times. It handles grain allocation and mixing of grains to form a
 * single output. */
class Scheduler
{
public:
    Scheduler(const map<string, AudioFile> &files, float sample_rate);

    /* Load audio file map into scheduler. */
    void load_files(const map<string, AudioFile> &new_files);

    /* Calculate grain activations. */
    float schedule(int marker, const string &filename);

    /* Set new grain attack value. */
    void set_grain_attack(float new_grain_attack);

    /* Set new grain release value. */
    void set_grain_release(float new_grain_release);

    /* Set new grain density value. */
    void set_grain_density(int new_grain_density);

    /* Set new grain size. */
    void set_grain_size(int new_grain_size);

private:
    /* Mix all currently active grains to a single output. */
    float synthesize_grains();

    /* Create a new grain object at next index of grain pool. */
    void create_grain(int marker, const string &file_name);

    /* Stochastically generate next inter-onset value. */
    int get_next_inter_onset();

    /* Fill buffer from filename and file position. */
    void fill_buffer(int marker, const string &file_name);

    int max_grains = 128;
    int grain_size = 256;
    int grain_density;
    float grain_width;
    float sample_rate;
    vector<Grain> grains;
    map<string, AudioFile> files;
    AudioBuffer buffer;
    EnvelopeParams env_params;
    Rand<float> rand;
    int next_onset;
    int grain_index;
};

} // CATE

#endif
