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
    explicit Granulator(float sample_rate);

    /* Load files from corpus into files variable. */
    void load_files(const Corpus &corpus);

    /* Get the next sample value from the granulator. */
    float synthesize(Unit unit);

    /* Set new grain attack value. */
    void set_grain_attack(float new_grain_attack);

    /* Set new grain sustain value. */
    void set_grain_sustain(float new_grain_sustain);

    /* Set new grain decay value. */
    void set_grain_release(float new_grain_release);

    /* Set new grain density value. */
    void set_grain_density(int new_grain_density);

    /* Set new grain size value. */
    void set_grain_size(int new_grain_size);

    /* Get grain size. */
    inline const int get_grain_size() const { return grain_params.get_grain_size(); }

private:
    GrainParams grain_params;
    EnvelopeParams envelope_params;
    Scheduler scheduler;
    float sample_rate;
};

} // CATE

#endif
