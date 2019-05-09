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

AudioBuffer read_audio_file(const Path &input_path)
{
    SndfileHandle file(input_path);

    if (file.error())
    {
        throw std::invalid_argument("Invalid audio file.\n");
    }

    auto size = file.frames() * file.channels();
    auto buffer = AudioBuffer(static_cast<unsigned long>(size));
    file.read(&buffer[0], size);

    return buffer;
}

void write_audio_file(const AudioBuffer &buffer, const Path &output_path, float sample_rate)
{
    const auto format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    const auto channels = 1;

    SndfileHandle file(output_path, SFM_WRITE, format, channels, static_cast<int>(sample_rate));

    if (file.error())
    {
        throw std::invalid_argument("Invalid audio file.\n");
    }

    file.write(&buffer[0], static_cast<sf_count_t>(buffer.size() / channels));
}

} // CATE
