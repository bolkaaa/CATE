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

/*
  The Synth class handles data used in the synthesis process, such as arrays of
  audio samples and analysis data. The data is stored in std::vectors, which
  have to be initialised with fixed sizes, since we cannot dynamically push
  data into them in the audio callback function.
 */

#ifndef SYNTH_HPP
#define SYNTH_HPP

#include <vector>

using std::vector;

class Synth
{
public:
    Synth(uint16_t fft_bin_size, uint16_t audio_buffer_size);

    Synth();

private:
    uint16_t fft_bin_size;
    uint16_t audio_buffer_size;
    vector<float> data;

};

#endif
