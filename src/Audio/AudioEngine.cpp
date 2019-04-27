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

AudioEngine::AudioEngine(float sample_rate, unsigned long frames_per_buffer, int input_channels,
                         int output_channels)
        : sample_rate(sample_rate),
          frames_per_buffer(frames_per_buffer),
          input_channels(input_channels),
          output_channels(output_channels),
          input_device(Pa_GetDefaultInputDevice()),
          output_device(Pa_GetDefaultOutputDevice())
{
    is_running = false;
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
    configure_inputs_outputs();
}

PaError AudioEngine::start_stream()
{
    if (is_running)
    {
        return -1;
    }

    is_running = true;

    error = Pa_OpenStream(&stream,
                          &input_params,
                          &output_params,
                          sample_rate,
                          frames_per_buffer,
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
    input_device = Pa_GetDefaultInputDevice();
    output_device = Pa_GetDefaultOutputDevice();
}

void AudioEngine::configure_inputs_outputs()
{
    input_params.device = input_device;
    input_params.channelCount = input_channels;
    input_params.sampleFormat = paFloat32;
    input_params.hostApiSpecificStreamInfo = nullptr;

    output_params.channelCount = output_channels;
    output_params.device = output_device;
    output_params.sampleFormat = paFloat32;
    output_params.hostApiSpecificStreamInfo = nullptr;
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
