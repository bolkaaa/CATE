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

Grain::Grain(const AudioBuffer &buffer, int size)
        : remaining_samples(size),
          size(size),
          src(buffer)
{
}

// TODO: Try remaining samples instead of size in env
float Grain::synthesize(float attack, float sustain, float release)
{
    float source_output = src.synthesize();
    float env_output = env.synthesize(size, attack, sustain, release);

    --remaining_samples;

    return source_output * env_output;
}

bool Grain::is_active() const
{
    return (remaining_samples > 0);
}


} // CATE
