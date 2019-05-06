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

#include "AnalysisWorker.hpp"
#include "src/Analysis/Feature.hpp"

namespace CATE {

AnalysisWorker::AnalysisWorker(AudioSettings *audio_settings)
        : audio_settings(audio_settings),
          fft(audio_settings)
{
}

void AnalysisWorker::input_data_received(RingBuffer *ring_buffer)
{
    ring_buffer->pop(buffer[counter]);

    ++counter;

    if (counter > buffer_size)
    {
        fft.fill(buffer);
        fft.compute_spectrum();
        counter -= buffer_size;
    }
}

} // CATE
