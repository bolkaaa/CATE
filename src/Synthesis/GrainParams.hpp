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

class GrainParams
{
public:
    GrainParams();

    /* Set maximum number of grains. */
    void set_max_grains(int max_grains);

    /* Set grain size. */
    void set_grain_size(int grain_size);

    /* Set grain density. */
    void set_grain_density(float grain_density);

    /* Set grain width. */
    void set_grain_width(float grain_width);

    /* Get maximum number of grains. */
    int get_max_grains() const;

    /* Get grain size. */
    int get_grain_size() const;

    /* Get grain density. */
    float get_grain_density() const;

    /* Get grain width. */
    float get_grain_width() const;

private:
    int max_grains;
    int grain_size;
    float grain_density;
    float grain_width;
};

#endif //GRAINPARAMS_HPP
