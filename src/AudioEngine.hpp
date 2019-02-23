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

#include <portaudio.h>

class AudioEngine
{
public:
    AudioEngine();

    ~AudioEngine();

    void init();

    int start_stream();

    int stop_stream();

private:

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

    PaError error;
    PaStream *stream;
    PaTime suggested_latency;
    PaHostApiIndex selected_host_api;
    PaStreamParameters input_params;
    PaStreamParameters output_params;
    double sample_rate;
    int frames_per_buffer;
    bool is_running;
};

#endif
