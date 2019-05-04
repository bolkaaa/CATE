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
#include "AudioFile.hpp"

namespace CATE {

RecordWorker::RecordWorker(AudioSettings *audio_settings)
        : audio_settings(audio_settings)
{
}

void RecordWorker::record_data_received(CATE::RingBuffer *ring_buffer)
{
    auto sample = 0.0f;

    if (ring_buffer->samples_available())
    {
        ring_buffer->pop(sample);
        record_data.data.emplace_back(sample);
    }
}

void RecordWorker::save_recording(const std::string &output_path, int channels)
{
    record_data.write(channels, audio_settings->get_sample_rate(), output_path);
}

} // CATE
