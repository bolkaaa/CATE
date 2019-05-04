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
#include <vector>
#include <string>

#include <portaudio.h>

#include "AudioSettings.hpp"

using std::vector;
using std::string;

namespace CATE {

/*
  The AudioEngine class wraps the PortAudio C library, providing the
  functionality for initialising PortAudio, and starting and stopping streams.
  It defines the callback functions, which the AudioProcess class creates its
  own versions of when it inherits from the base AudioEngine class.
 */
class AudioEngine
{
public:
    AudioEngine(AudioSettings *audio_settings);

    ~AudioEngine();

    /* Start a PortAudio stream. */
    int start_stream();

    /* Stop the PortAudio stream. */
    int stop_stream();

    /* Get list of names of audio devices. */
    vector<string> get_available_devices();

    /* Get default input audio device index. */
    const int get_default_input_device() const { return Pa_GetDefaultInputDevice(); }

    /* Get default output audio device index. */
    const int get_default_output_device() const { return Pa_GetDefaultOutputDevice(); }

    /* Get number of input channels of current input device. */
    const int get_num_input_channels();

    /* Get number of output channels of current input device. */
    const int get_num_output_channels();

    /* Set current input device through index of available devices. */
    void set_input_device(int selection_index);

    /* Set current output device through index of available devices. */
    void set_output_device(int selection_index);

    /* Given a PaError code, return the error text. */
    inline string report_error(PaError error_code) const { return string(Pa_GetErrorText(error_code)); }

private:
    /* Run initialisation commands. */
    void init();

    /* Configure inputs and outputs. */
    void configure_stream_parameters();

protected:
    /* The main processing callback function. */
    virtual int processing_callback(const void *input_buffer,
                                    void *output_buffer,
                                    unsigned long frames_per_buffer,
                                    const PaStreamCallbackTimeInfo *time_info,
                                    PaStreamCallbackFlags status_flags);

    /* With PortAudio, a static callback function must be defined, which returns the custom callback function. */
    static int static_callback(const void *input_buffer,
                               void *output_buffer,
                               unsigned long frames_per_buffer,
                               const PaStreamCallbackTimeInfo *time_info,
                               PaStreamCallbackFlags status_flags,
                               void *user_data)
    {
        return static_cast<AudioEngine *>(user_data)->processing_callback(input_buffer,
                                                                          output_buffer,
                                                                          frames_per_buffer,
                                                                          time_info,
                                                                          status_flags);
    }

    PaStream *stream;
    AudioSettings *audio_settings;
    PaStreamParameters input_parameters;
    PaStreamParameters output_parameters;
    PaDeviceIndex input_device;
    PaDeviceIndex output_device;
    PaError error;
    bool is_running;
};

} // CATE

#endif
