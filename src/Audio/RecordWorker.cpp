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

#include "RecordWorker.hpp"

namespace CATE {

RecordWorker::RecordWorker(AudioSettings *audio_settings)
        : audio_settings(audio_settings)
{
}

void RecordWorker::output_data_received(RingBuffer<float> *ring_buffer)
{
    while (ring_buffer->samples_available() > 0)
    {
        auto data = 0.0f;
        ring_buffer->pop(data);
        recording_data.push_front(data);
    }
}

void RecordWorker::save_recording(const Path &output_path)
{
    AudioBuffer output_buffer(recording_data.begin(), recording_data.end());
    write_audio_file(output_buffer, output_path, audio_settings->get_sample_rate().value);
    recording_data.clear();
}

} // CATE
