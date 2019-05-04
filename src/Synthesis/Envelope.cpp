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

Envelope::Envelope(const EnvelopeParams &params)
        : index(0),
          amp(0.0f),
          amp_incr(0.0f),
          params(params)
{
}

float Envelope::synthesize()
{
    auto attack_samples = static_cast<int>(params.get_sample_size() * params.get_attack());
    auto release_samples = static_cast<int>(params.get_sample_size() * params.get_release());

    if (index < attack_samples) // attack
    {
        amp_incr = params.get_sustain() / attack_samples;
    }
    else if (attack_samples < index && index < release_samples) // sustain
    {
        amp_incr = 0.0f;
    }
    else // release
    {
        amp_incr = -(params.get_sustain() / release_samples);
    }

    ++index;
    amp += amp_incr;

    return amp;
}


} // CATE
