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

#ifndef AUDIO_PARAMETERS_HPP
#define AUDIO_PARAMETERS_HPP

#include "portaudiocpp/PortAudioCpp.hxx"

class AudioParameters
{
public:
    /* Use defaults for audio system settings. */
    AudioParameters(portaudio::System &system);

    /* Use provided arguments for audio system settings. */
    AudioParameters(portaudio::System &system, uint64_t sample_rate,
                    uint32_t frames_per_buffer,
                    uint8_t input_channels, uint8_t output_channels);

    uint64_t sample_rate() { return sr; }

    uint32_t frames_per_buffer() { return frames_per_buf; }

    uint8_t in_channels() { return in_chan; }

    uint8_t out_channels() { return out_chan; }

    portaudio::StreamParameters stream() { return strm; }

private:
    /* Set stream objects from parameters. */
    void set(portaudio::System &system);

    uint64_t sr;
    uint32_t frames_per_buf;
    uint8_t in_chan;
    uint8_t out_chan;
    portaudio::DirectionSpecificStreamParameters in;
    portaudio::DirectionSpecificStreamParameters out;
    portaudio::StreamParameters strm;
};

#endif
