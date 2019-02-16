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

#include "AudioParameters.hpp"

AudioParameters::AudioParameters(portaudio::System &system)
    : sr(44100), frames_per_buf(256), in_chan(2), out_chan(2)
{
    set(system);
}

AudioParameters::AudioParameters(portaudio::System &system, uint64_t sr,
                                 uint32_t frames_per_buf,
                                 uint8_t in_chan, uint8_t out_chan)
    : sr(sr), frames_per_buf(frames_per_buf), in_chan(in_chan), out_chan(out_chan)
{
    set(system);
}

void AudioParameters::set(portaudio::System &system)
{
    in = portaudio::DirectionSpecificStreamParameters(
        system.defaultInputDevice(),
        in_chan,
        portaudio::FLOAT32,
        false,
        system.defaultInputDevice().defaultLowInputLatency(),
        nullptr);

    out = portaudio::DirectionSpecificStreamParameters(
        system.defaultOutputDevice(),
        out_chan,
        portaudio::FLOAT32,
        false,
        system.defaultOutputDevice().defaultLowOutputLatency(),
        nullptr);

    strm = portaudio::StreamParameters(
        in,
        out,
        sr,
        frames_per_buf,
        paClipOff);
}
