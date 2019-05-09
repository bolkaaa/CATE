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
#include <unordered_map>

#include <sndfile.hh>
#include <boost/functional/hash.hpp>

#include "src/Corpus/PathTree.hpp"

namespace CATE {

/* Typedef that aliases a std::vector of real values. For simplicity's sake and compatibility with C
 * libraries used in CATE, it just uses floats and isn't templated. */
typedef std::vector<float> AudioBuffer;
/* Path to audio file and associated audio data. */
typedef std::unordered_map<Path, AudioBuffer> AudioBufferMap;
/* Key of file position index and audio file path, and value of associated audio data. */
typedef std::unordered_map<std::pair<int, Path>, AudioBuffer, boost::hash<std::pair<int, Path>>> AudioFrameMap;

/* Given an audio file path, return an AudioBuffer containing its contents. */
AudioBuffer read_audio_file(const Path &input_path);

/* Write AudioBuffer to a WAV file at the specified path. */
void write_audio_file(const AudioBuffer &buffer, const Path &output_path, float sample_rate);

} // CATE

#endif
