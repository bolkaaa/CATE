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
#include "src/Util/Util.hpp"

namespace CATE {

GrainParams::GrainParams()
        : grain_size(default_grain_size),
          grain_density(default_grain_density)
{
    max_grains = get_default_max_grains();
}

void GrainParams::set_max_grains(int selector_index)
{
    if (valid_input(selector_index, available_max_grains.size()))
    {
        max_grains = available_max_grains[selector_index];
    }
}

} // CATE
