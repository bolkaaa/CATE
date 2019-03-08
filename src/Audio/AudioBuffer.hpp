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

#ifndef AUDIO_BUFFER_HPP
#define AUDIO_BUFFER_HPP

#include <vector>
#include <map>

using std::vector;
using std::map;

namespace CATE {

/* AudioBuffer is a typedef that aliases a vector of real values. For simplicity's sake and compatibility with C
 * libraries used in CATE, it just uses floats and isn't templated. */
typedef std::vector<float> AudioBuffer;

/* Given an AudioBuffer, segment into a std::map where the key of each subset is its frame position in samples, and the
 * value of each subset is an AudioBuffer of size <frame_size>. */
map<int, AudioBuffer> segment_frames(const AudioBuffer &source, int frame_size);

} // CATE

#endif
