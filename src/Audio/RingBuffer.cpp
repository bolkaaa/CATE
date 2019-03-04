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

#include <algorithm>

#include "AudioBuffer.hpp"
#include "RingBuffer.hpp"

RingBuffer::RingBuffer(int size)
    : AudioBuffer(size), data(AudioBuffer::data),
      head(0), tail(0),
      high_water_mark(static_cast<int>(data.size() / 4))
{
    high_water_mark = static_cast<int>((high_water_mark > data.size()) ? data.size() : high_water_mark);
}

void RingBuffer::push(float elem)
{
    if (space_available())
    {
        data[tail] = elem;
        tail = static_cast<int>((tail + 1) % data.size());
    }
}

void RingBuffer::pop(float &ref)
{
    if (samples_available())
    {
        ref = data[head];
        head = static_cast<int>((head + 1) % data.size());
    }
}

int RingBuffer::samples_available()
{
    auto samples = static_cast<int>((tail - head + data.size()) % data.size());

    return samples;
}

int RingBuffer::space_available()
{
    int free = static_cast<int>((head - tail + data.size() - 1) % data.size());
    int undermark = high_water_mark - samples_available();
    auto space = std::min(undermark, free);

    return space;
}
