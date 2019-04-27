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
#include <random>

#include "src/Audio/AudioFile.hpp"
#include "Grain.hpp"
#include "EnvelopeParams.hpp"

using std::vector;
using std::array;
using std::random_device;
using std::mt19937;
using std::uniform_real_distribution;

namespace CATE {

/* Handles activation of grains, based upon onset times. It handles grain allocation and mixing of grains to form a
 * single output. */
class Scheduler
{
public:
    Scheduler(const map<string, AudioFile> &files, float sample_rate);

    void load_files(const map<string, AudioFile> &new_files);

    /* Calculate grain activations. */
    float schedule(int marker, const string &filename);

    /* Set new grain attack value. */
    void set_grain_attack(float new_grain_attack);

    /* Set new grain release value. */
    void set_grain_release(float new_grain_release);

    /* Set new grain density value. */
    void set_grain_density(int new_grain_density);

    /* Set new grain width value. */
    void set_grain_width(int new_grain_width);

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

    int max_grains = 32;
    int grain_size = 1024;
    vector<Grain> grains;
    map<string, AudioFile> files;
    AudioBuffer buffer;
    random_device seed;
    mt19937 gen;
    uniform_real_distribution<float> dist;
    EnvelopeParams env_params;
    int grain_density;
    float grain_width;
    int next_onset;
    int grain_index;
    float sample_rate;
};

} // CATE

#endif
