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

#include "src/Audio/AudioSettings.hpp"
#include "Source.hpp"
#include "Envelope.hpp"

namespace CATE {

/* Grain combines an Envelope and Source to represent a single grain in the
 * granulation system. */
class Grain
{
public:
    Grain();

    Grain(const AudioBuffer &data, AudioSettings *audio_settings);

    /* Activate grain, setting remaining samples to a particular value. */
    void activate(float size, float pitch, float attack, float sustain, float release);

    /* Synthesize next sample from audio source and envelope. */
    float synthesize();

    /* Return active status. */
    bool is_active() const;

private:
    int sample_size;
    int remaining_samples;
    float phase_incr;
    float phase;
    float pitch;
    float size;
    float attack;
    float sustain;
    float release;
    Source src;
    Envelope env;
    AudioSettings *audio_settings;
};

} // CATE

#endif
