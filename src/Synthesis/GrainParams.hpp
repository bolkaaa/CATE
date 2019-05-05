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

#ifndef GRAINPARAMS_HPP
#define GRAINPARAMS_HPP

#include <vector>

using std::vector;

namespace CATE {

typedef vector<int> MaxGrainsVector;

class GrainParams
{
public:
    GrainParams();

    /* Get parameters. */
    int get_max_grains() const { return max_grains; }
    int get_grain_size() const { return grain_size; }
    float get_grain_density() const { return grain_density; }
    const int get_default_max_grains() const { return available_max_grains[default_max_grains_index]; }
    const int get_default_max_grains_index() const { return default_max_grains_index; }
    const int get_default_grain_size() const { return default_grain_size; }
    const float get_default_grain_density() const { return default_grain_density; }
    inline const MaxGrainsVector get_available_max_grains() const { return available_max_grains; }

    /* Set parameters. */
    void set_max_grains(int selector_index);
    void set_grain_size(int new_grain_size) { grain_size = new_grain_size; }
    void set_grain_density(float new_grain_density) { grain_density = new_grain_density; }

private:
    const MaxGrainsVector available_max_grains = { 8, 16, 24, 32, 48, 64, 128 };
    const int default_max_grains_index = 3;
    const int default_grain_size = 128;
    const float default_grain_density = 100.0f;
    int max_grains;
    int grain_size;
    float grain_density;
};

} // CATE

#endif //GRAINPARAMS_HPP
