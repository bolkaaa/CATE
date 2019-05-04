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

#include "GrainParams.hpp"

namespace CATE {

GrainParams::GrainParams()
        : max_grains(32),
          grain_size(256),
          grain_density(100),
          grain_width(0.5)
{
}

int GrainParams::get_max_grains() const
{
    return max_grains;
}

int GrainParams::get_grain_size() const
{
    return grain_size;
}

float GrainParams::get_grain_density() const
{
    return grain_density;
}

float GrainParams::get_grain_width() const
{
    return grain_width;
}

void GrainParams::set_max_grains(int max_grains)
{
    GrainParams::max_grains = max_grains;
}

void GrainParams::set_grain_size(int grain_size)
{
    GrainParams::grain_size = grain_size;
}

void GrainParams::set_grain_density(float grain_density)
{
    GrainParams::grain_density = grain_density;
}

void GrainParams::set_grain_width(float grain_width)
{
    GrainParams::grain_width = grain_width;
}

} // CATE
