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

#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

#include <algorithm>

#include "AudioBuffer.hpp"

namespace CATE {

/* Based on lock-free ring buffer implementation in "Audio Anecdotes Volume 2
 * (Greenebaum, Barzel)". */
template <class T>
class RingBuffer
{
public:
    /* Allocate memory for ring buffer. */
    explicit RingBuffer(int size);

    /* Insert element at head. */
    void push(T elem);

    /* Get tail element. */
    void pop(T &output);

    /* Calculate samples available in buffer. */
    int samples_available();

    /* Calculate space left in buffer. */
    int space_available();

private:
    vector<T> data;
    int head;
    int tail;
    int high_water_mark;
};

template <class T>
RingBuffer<T>::RingBuffer(int size)
        : data(size),
          head(0), tail(0),
          high_water_mark(static_cast<int>(data.size() / 4))
{
    high_water_mark = static_cast<int>((high_water_mark > data.size()) ? data.size() : high_water_mark);
}

template <class T>
void RingBuffer<T>::push(T elem)
{
    if (space_available())
    {
        data[tail] = elem;
        tail = static_cast<int>((tail + 1) % data.size());
    }
}

template <class T>
void RingBuffer<T>::pop(T &ref)
{
    if (samples_available())
    {
        ref = data[head];
        head = static_cast<int>((head + 1) % data.size());
    }
}

template <class T>
int RingBuffer<T>::samples_available()
{
    auto samples = static_cast<int>((tail - head + data.size()) % data.size());
    return samples;
}

template <class T>
int RingBuffer<T>::space_available()
{
    auto free = static_cast<int>((head - tail + data.size() - 1) % data.size());
    auto undermark = high_water_mark - samples_available();
    auto space = std::min(undermark, free);
    return space;
}

} // CATE

#endif