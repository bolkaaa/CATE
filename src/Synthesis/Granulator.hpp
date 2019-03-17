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

#ifndef GRANULATOR_HPP
#define GRANULATOR_HPP

#include <vector>
#include <src/Audio/AudioFile.hpp>

#include "./Scheduler.hpp"

namespace CATE {

/* Granulator is the high-level interface that contains all the functionality
 * for the granular synthesis process.  */
class Granulator
{
public:
    Granulator(vector<AudioFile> files, float sample_rate);

    /* Get the next sample value from the granulator. */
    float synthesize();

private:
    vector<AudioFile> files;
    Scheduler scheduler;
    float sample_rate;
};

} // CATE

#endif