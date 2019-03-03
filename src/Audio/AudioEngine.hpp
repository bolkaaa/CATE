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

#ifndef AUDIO_ENGINE_HPP
#define AUDIO_ENGINE_HPP

#include <cstdlib>

#include <portaudio.h>

/*
  The AudioEngine class wraps the PortAudio C library, providing the
  functionality for initialising PortAudio, and starting and stopping streams.
  It defines the callback functions, which the AudioProcess class creates its
  own versions of when it inherits from the base AudioEngine class.
 */

class AudioEngine
{
public:
    AudioEngine(double sample_rate, unsigned long frames_per_buffer);

    ~AudioEngine();

    void init();

    int start_stream();

    int stop_stream();

protected:
    /* The main processing callback function. */
    virtual int processing_callback(const void *input_buffer,
                                    void *output_buffer,
                                    unsigned long frames_per_buffer,
                                    const PaStreamCallbackTimeInfo* time_info,
                                    PaStreamCallbackFlags status_flags);

    /* With PortAudio, a static callback function must be defined, which
       returns the custom callback function. */
    static int static_callback(const void *input_buffer,
                               void *output_buffer,
                               unsigned long frames_per_buffer,
                               const PaStreamCallbackTimeInfo* time_info,
                               PaStreamCallbackFlags status_flags,
                               void *user_data)
    {
        return ((AudioEngine*) user_data)->processing_callback(input_buffer,
                                                               output_buffer,
                                                               frames_per_buffer,
                                                               time_info,
                                                               status_flags);
    }

    PaStream *stream;
    PaError error;
    PaTime suggested_latency;
    PaStreamParameters input_params;
    PaStreamParameters output_params;
    double sample_rate;
    unsigned long frames_per_buffer;
    bool is_running;
};

#endif
