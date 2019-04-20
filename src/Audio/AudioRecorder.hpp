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

#ifndef AUDIORECORD_HPP
#define AUDIORECORD_HPP

#include "AudioBuffer.hpp"

namespace CATE {

class AudioRecorder
{
public:
    AudioRecorder(float sample_rate);

    /* Write audio sample to audio buffer. */
    void write(float sample);

    /* Save recording data to output file. */
    void save(const std::string &output_path, int num_channels, float sample_rate);

private:
    float sample_rate;
    const float max_recording_minutes = 60.0f;
    int max_recording_size;
    int record_pos;
    AudioBuffer buffer;

};

} // CATE

#endif
