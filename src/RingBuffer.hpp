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

/* Based on lock-free ring buffer implementation in "Audio Anecdotes Volume 2 
   (Greenebaum, Barzel)". */

template <class T>
class RingBuffer
{
public:
    /* Allocate memory for buffer. */
    RingBuffer<T>();

    /* Allocate and set parameters. */
    RingBuffer<T>(uint32_t sz, uint32_t high_water_mark);

    /* Insert element at head. */
    void push(T elem);

    /* Get tail element. */
    void pop(T &ref);

    uint32_t size() const { return sz; }

    /* Index into buffer data. */
    T operator [](const uint32_t i) { return data[i]; }

    /* Calculate samples available in buffer. */
    uint32_t samples_available();

    /* Calculate space left in buffer. */
    uint32_t space_available();

private:
    T *data;
    uint32_t head;
    uint32_t tail;
    uint32_t high_water_mark;
    const uint32_t sz;
};

template <class T>
RingBuffer<T>::RingBuffer()
    : sz(4064), high_water_mark(1024)
{
    data = new T[sz];

    for (uint32_t i = 0; i < sz; ++i)
    {
        data[i] = 0;
    }

    high_water_mark = (high_water_mark > sz) ? sz : high_water_mark;
}

template <class T>
RingBuffer<T>::RingBuffer(uint32_t sz, uint32_t high_water_mark)
    : sz(sz), head(0), tail(0), high_water_mark(high_water_mark)
{
    data = new T[sz];

    for (uint32_t i = 0; i < sz; ++i)
    {
        data[i] = 0;
    }

    high_water_mark = (high_water_mark > sz) ? sz : high_water_mark;
}

template <class T>
void RingBuffer<T>::push(T elem)
{
    if (space_available())
    {
        data[tail] = elem;
        tail = (tail + 1) % sz;
    }
}

template <class T>
void RingBuffer<T>::pop(T &ref)
{
    if (samples_available())
    {
        ref = data[head];
        head = (head + 1) % sz;
    }
}

template <class T>
uint32_t RingBuffer<T>::samples_available()
{
    uint32_t samples = (tail - head + sz) % sz;

    return samples;
}

template <class T>
uint32_t RingBuffer<T>::space_available()
{
    uint32_t free = (head - tail + sz - 1) % sz;
    uint32_t undermark = high_water_mark - samples_available();
    uint32_t space = std::min(undermark, free);

    return space;
}

#endif
