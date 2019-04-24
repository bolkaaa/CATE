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

#ifndef SOURCE_HPP
#define SOURCE_HPP

#include "src/Audio/AudioFile.hpp"

namespace CATE {

/* Source represents the extraction of sample data from a source (in this case, an AudioFile object which contains a
 * buffer of sample data), to be passed to the Grain object along with an Envelope object. */
class Source
{
public:
    Source(const AudioBuffer &buffer);

    /* Get next sample value from source. */
    float synthesize();

private:
    AudioBuffer buffer;
    int index;
};

} // CATE

#endif
