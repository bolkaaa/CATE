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
#include "EnvelopeParams.hpp"
#include "Scheduler.hpp"

using std::map;

namespace CATE {

/* Granulator is the high-level interface that contains all the functionality
 * for the granular synthesis process. The audio files within the set of audio files are stored in memory as a
 * std::map, so the audio data can be retrieved through providing the filename.
 * */
class Granulator
{
public:
    Granulator(map<string, AudioFile> files, float sample_rate);

    /* Get the next sample value from the granulator. */
    float synthesize(int marker, string filename);

    /* Set new grain attack value (0 - 1.0). */
    void set_grain_attack(float new_grain_attack);

    /* Set new grain decay value (0 - 1.0). */
    void set_grain_release(float nre_grain_release);

    /* Set new grain attack value (0 - 127). */
    void set_grain_density(int new_grain_density);

private:
    map<string, AudioFile> files;
    Scheduler scheduler;
    const int max_grains = 32;
    const int buffer_size = 4096;
    float sample_rate;
};

} // CATE

#endif
