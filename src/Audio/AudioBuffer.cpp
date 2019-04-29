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

#include <iostream>

#include "AudioBuffer.hpp"

namespace CATE {

map<int, AudioBuffer> segment_frames(const AudioBuffer &source, int frame_size)
{
    map<int, AudioBuffer> frames;
    auto n = source.size();
    auto remaining_space = n % frame_size;
    auto frame_counter = 0;

    for (auto it = source.begin(); it != (source.end() - remaining_space); it += frame_size)
    {
        AudioBuffer segment_data(it, it + frame_size);
        std::pair<int, AudioBuffer> segment = {frame_counter, segment_data};
        frames.insert(segment);
        frame_counter += frame_size;
    }

    return frames;
}

} // CATE
