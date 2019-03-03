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

#include "Util.hpp"

namespace Util {

float samps_to_ms(uint32_t num_samples, uint16_t sample_rate)
{
    float s = static_cast<float>(num_samples) / sample_rate;
    float ms = s * 1000;
    return ms;
}

uint32_t ms_to_samps(float ms, uint16_t sample_rate)
{
    float s = ms / 1000;
    uint32_t samps = s * sample_rate;
    return samps;
}

} // Util
