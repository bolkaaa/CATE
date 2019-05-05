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

#ifndef GRANULATOR_HPP
#define GRANULATOR_HPP

#include <map>

#include "src/Audio/AudioFile.hpp"
#include "src/Corpus/Corpus.hpp"
#include "Scheduler.hpp"

namespace CATE {

/* Representation of position in audio file. */
class Unit
{
public:
    string file_path;
    int marker;
};

/* Granulator is the high-level interface that contains all the functionality for the granular synthesis process. */
class Granulator
{
public:
    explicit Granulator(AudioSettings *audio_settings);

    /* Reallocate memory for grain objects according to size changes. */
    void rebuild_grain_pool();

    /* Load files from corpus into files variable. */
    void load_files(Corpus *corpus);

    /* Get the next sample value from the granulator. */
    float synthesize(Unit unit);

    /* Return status of whether granulator has loaded files. */
    bool is_ready() { return has_files; }

    /* Set parameters. */
    void set_grain_attack(float value) { grain_attack->value = value; }
    void set_grain_sustain(float value) { grain_sustain->value = value; }
    void set_grain_release(float value) { grain_release->value = value; }
    void set_grain_size(int value) { grain_size->value = value; }
    void set_grain_density(float value) { grain_density->value = value; }
    void set_max_grains(int selection_index) { max_grains->set(selection_index); }

    /* Get parameters. */
    const Param<float> get_grain_attack() { return *(grain_attack); }
    const Param<float> get_grain_sustain() { return *(grain_sustain); }
    const Param<float> get_grain_release() { return *(grain_release); }
    const Param<int> get_grain_size() { return *(grain_size); }
    const Param<float> get_grain_density() { return *(grain_density); }
    const FixedParam<int> get_max_grains() { return *(max_grains); }

private:
    Param<float> *grain_attack;
    Param<float> *grain_sustain;
    Param<float> *grain_release;
    Param<float> *grain_density;
    Param<int> *grain_size;
    FixedParam<int> *max_grains;
    AudioSettings *audio_settings;
    Scheduler scheduler;
    bool has_files;
};

} // CATE

#endif
