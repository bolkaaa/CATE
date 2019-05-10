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

#include <iostream>
#include <cmath>

#include "Envelope.hpp"

namespace CATE {

Envelope::Envelope()
        : index(0),
          amp(0.0f),
          amp_incr(0.0f)
{
}

void Envelope::reset()
{
    index = 0;
    amp = 0.0f;
    amp_incr = 0.0f;
}

float Envelope::synthesize(int size, int phase, float attack, float sustain, float release)
{
    auto attack_samples = static_cast<int>(size * attack);
    auto release_samples = static_cast<int>(size * release);

    if (index < attack_samples) // attack
    {
        amp_incr = sustain / attack_samples;
    }
    else if (index > release_samples) // release
    {
        amp_incr = -(sustain / release_samples);
    }
    else // sustain
    {
        amp_incr = 0.0f;
    }

    amp += amp_incr;
    index += phase;

    return amp;
}


} // CATE
