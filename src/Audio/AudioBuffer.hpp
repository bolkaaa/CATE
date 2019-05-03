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
using std::pair;

namespace CATE {

/* AudioBuffer is a typedef that aliases a vector of real values. For simplicity's sake and compatibility with C
 * libraries used in CATE, it uses floats and isn't templated. */
typedef std::vector<float> AudioBuffer;

/* Alias for individual frame of audio, where the key is a positional marker and the value is audio data. */
typedef pair<int, AudioBuffer> AudioFrame;

/* Alias for std::map where the key is frame position in samples and the value is the audio data. */
typedef map<int, AudioBuffer> AudioFramePool;

/* Given an AudioBuffer, segment into an AudioFrameSet. */
AudioFramePool segment_frames(const AudioBuffer &source, int frame_size);

} // CATE

#endif
