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

#ifndef AUDIO_PROCESS_HPP
#define AUDIO_PROCESS_HPP

#include <QObject>

#include "AudioEngine.hpp"
#include "Synth.hpp"

/*
The audio processing occurs in AudioProcess, which inherits from the audio
engine class that wraps the PortAudio functionality. It has a Synth object as a
member that contains the data used by the callback function
(processing_callback).
 */

class AudioProcess : public QObject, public AudioEngine
{
    Q_OBJECT

public:
    AudioProcess(uint16_t sample_rate, uint16_t frames_per_buffer,
                 uint16_t fft_bin_size);

    ~AudioProcess();

    Synth *synth;

protected:
    virtual int processing_callback(const void *input_buffer,
                                    void *output_buffer,
                                    unsigned long frames_per_buffer,
                                    const PaStreamCallbackTimeInfo* time_info,
                                    PaStreamCallbackFlags status_flags);
};

#endif
