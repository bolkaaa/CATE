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

namespace CATE {

float samps_to_ms(int num_samples, double sample_rate)
{
    auto s = static_cast<float>(num_samples) / sample_rate;
    auto ms = static_cast<float>(s * 1000);

    return ms;
}

int ms_to_samps(float ms, double sample_rate)
{
    float s = ms / 1000;
    auto samps = static_cast<int>(s * sample_rate);

    return samps;
}

} // CATE
