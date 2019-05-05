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
#include "EnvelopeParams.hpp"
#include "GrainParams.hpp"
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
    Granulator(AudioSettings *audio_settings, GrainParams grain_params, EnvelopeParams env_params);

    /* Reallocate memory for grain objects according to size changes. */
    void rebuild_grain_pool();

    /* Load files from corpus into files variable. */
    void load_files(Corpus *corpus);

    /* Get the next sample value from the granulator. */
    float synthesize(Unit unit);

    /* Return status of whether granulator has loaded files. */
    bool is_ready() { return has_files; }

    /* Set parameters. */
    void set_grain_attack(float attack) { scheduler.set_grain_attack(attack); }
    void set_grain_sustain(float sustain) { scheduler.set_grain_sustain(sustain); }
    void set_grain_release(float release) { scheduler.set_grain_release(release); }
    void set_grain_size(int size) { scheduler.set_grain_size(size); env_params.set_sample_size(size); }
    void set_grain_density(float density) { scheduler.set_grain_density(density); }
    void set_max_grains(int selection_index) { scheduler.set_max_grains(selection_index); }

private:
    AudioSettings *audio_settings;
    GrainParams grain_params;
    EnvelopeParams env_params;
    Scheduler scheduler;
    bool has_files;
};

} // CATE

#endif
