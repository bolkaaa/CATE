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

#include "AudioEngine.hpp"

#include <cstdlib>
#include <cstdio>
#include <iostream>

namespace CATE {

AudioEngine::AudioEngine(AudioSettings &audio_settings)
        : audio_settings(audio_settings),
          is_running(false)
{
    init();
}

AudioEngine::~AudioEngine()
{
    Pa_Terminate();
}

int AudioEngine::processing_callback(const void *input_buffer,
                                     void *output_buffer,
                                     unsigned long frames_per_buffer,
                                     const PaStreamCallbackTimeInfo *time_info,
                                     PaStreamCallbackFlags status_flags)
{
    static_cast<void>(input_buffer);
    static_cast<void>(output_buffer);
    static_cast<void>(frames_per_buffer);
    static_cast<void>(time_info);
    static_cast<void>(status_flags);

    return paContinue;
}

void AudioEngine::init()
{
    error = Pa_Initialize();

    if (error != paNoError)
    {
        std::cerr << "PortAudio Error: " << Pa_GetErrorText(error) << "\n";
    }

    use_default_devices();
    configure_stream_parameters();
}

PaError AudioEngine::start_stream()
{
    if (is_running)
    {
        return -1;
    }

    is_running = true;

    error = Pa_OpenStream(&stream,
                          &input_parameters,
                          &output_parameters,
                          audio_settings.get_sample_rate(),
                          audio_settings.get_buffer_size(),
                          paNoFlag,
                          &AudioEngine::static_callback,
                          this);

    if (error != paNoError)
    {
        std::cerr << Pa_GetErrorText(error);
        is_running = false;
        return error;
    }

    error = Pa_StartStream(stream);

    if (error != paNoError)
    {
        std::cerr << Pa_GetErrorText(error);
        is_running = false;
    }

    return error;
}

int AudioEngine::stop_stream()
{
    error = -1;

    if (is_running)
    {
        error = Pa_AbortStream(stream);

        is_running = false;

        if (error != paNoError)
        {
            return error;
        }

        error = Pa_CloseStream(stream);
    }

    return error;
}

void AudioEngine::use_default_devices()
{
    audio_settings.set_input_device(Pa_GetDefaultInputDevice());
    audio_settings.set_output_device(Pa_GetDefaultOutputDevice());
}

void AudioEngine::configure_stream_parameters()
{
    input_parameters.device = audio_settings.get_input_device();
    input_parameters.channelCount = Pa_GetDeviceInfo(audio_settings.get_input_device())->maxInputChannels;
    input_parameters.sampleFormat = paFloat32;
    input_parameters.hostApiSpecificStreamInfo = nullptr;

    output_parameters.device = audio_settings.get_output_device();
    output_parameters.channelCount = Pa_GetDeviceInfo(audio_settings.get_output_device())->maxOutputChannels;
    output_parameters.sampleFormat = paFloat32;
    output_parameters.hostApiSpecificStreamInfo = nullptr;
}

vector<string> AudioEngine::get_device_list()
{
    int num_devices = Pa_GetDeviceCount();
    vector<string> device_list(num_devices);

    for (auto i = 0; i < num_devices; ++i)
    {
        const PaDeviceInfo *device_info = Pa_GetDeviceInfo(i);
        device_list[i] = device_info->name;
    }

    return device_list;
}


} // CATE
