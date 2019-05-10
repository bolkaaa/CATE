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

#include <cstdio>

#include "Grain.hpp"

namespace CATE {

Grain::Grain()
        : src(AudioBuffer()),
          remaining_samples(0)
{
}

Grain::Grain(const AudioBuffer &data, AudioSettings *audio_settings)
        : src(Source(data)),
          remaining_samples(0),
          phase(0.0f),
          phase_incr(0.0f),
          audio_settings(audio_settings)
{
}

void Grain::activate(float size, float pitch, float attack, float sustain, float release)
{
    src.reset();
    env.reset();
    phase = 0.0f;

    Grain::size = size;
    Grain::attack = attack;
    Grain::sustain = sustain;
    Grain::release = release;
    Grain::pitch = pitch;

    sample_size = static_cast<int>((size * audio_settings->get_sample_rate().value) / 1000.0f);
    phase_incr = size * pitch * 1.0f/audio_settings->get_sample_rate().value;
    remaining_samples = sample_size;
}

float Grain::synthesize()
{
    auto source_output = src.synthesize(phase);
    auto env_output = env.synthesize(sample_size, phase, attack, sustain, release);

    phase += phase_incr;

    remaining_samples -= phase;

    return source_output * env_output;
}

bool Grain::is_active() const
{
    return (remaining_samples > 0);
}


} // CATE
