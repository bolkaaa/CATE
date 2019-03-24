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

#ifndef GRAIN_HPP
#define GRAIN_HPP

#include "src/Audio/AudioFile.hpp"
#include "Source.hpp"
#include "Envelope.hpp"

namespace CATE {

/* Grain combines an Envelope and Source to represent a single grain in the
 * granulation system. */
class Grain
{
public:
    Grain();

    Grain(float dur, float sustain, float sample_rate, AudioBuffer buffer);

    /* Synthesize sample from audio source and envelope. */
    float synthesize();

    /* Return active status. */
    bool is_active() const;

private:
    float dur;
    float sustain;
    float sample_rate;
    int sample_size;
    int remaining_samples;
    bool active;
    Source src;
    Envelope env;
};

} // CATE

#endif
