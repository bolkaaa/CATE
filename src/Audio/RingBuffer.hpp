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

/* Based on lock-free ring buffer implementation in "Audio Anecdotes Volume 2
 * (Greenebaum, Barzel)". Will be used if I do some concurrent audio processing
 * later on (not doing so at the moment). */

namespace CATE {

class RingBuffer
{
public:
    /* Allocate memory for ring buffer. */
    explicit RingBuffer(int size);

    /* Insert element at head. */
    void push(float elem);

    /* Get tail element. */
    void pop(float &output);

    /* Calculate samples available in buffer. */
    int samples_available();

    /* Calculate space left in buffer. */
    int space_available();

private:
    AudioBuffer data;
    int head;
    int tail;
    int high_water_mark;
};

} // CATE

#endif
