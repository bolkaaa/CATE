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

/* TODO: Improve the error handling in these functions. */
AudioEngine::AudioEngine(uint16_t sample_rate, uint16_t frames_per_buffer)
    : sample_rate(sample_rate),
      frames_per_buffer(frames_per_buffer)
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
                                     const PaStreamCallbackTimeInfo* time_info,
                                     PaStreamCallbackFlags status_flags)
{
    static_cast<void>(input_buffer);
    static_cast<void>(output_buffer);
    static_cast<void>(frames_per_buffer);
    static_cast<void>(time_info);
    static_cast<void>(status_flags);

    return paContinue;
}

/* TODO: Clean up this code and separate into multiple functions. */
void AudioEngine::init()
{
    int device = Pa_GetDefaultOutputDevice();
    int audio_device_count = Pa_GetDeviceCount();
    const PaDeviceInfo* device_info;
    error = Pa_Initialize();

    if (error != paNoError)
    {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(error) << "\n";
    }

    for (int i = 0; i < audio_device_count; ++i)
    {
        device_info = Pa_GetDeviceInfo(i);
        std::cout << "Device " << i << ": " << device_info->name << "\n";
        if (i == device)
        {
            suggested_latency = device_info->defaultLowInputLatency;
        }
    }

    input_params.channelCount = 1;
    input_params.device = Pa_GetDefaultInputDevice();
    input_params.sampleFormat = paFloat32;
    input_params.suggestedLatency = suggested_latency;
    input_params.hostApiSpecificStreamInfo = NULL;

    output_params.channelCount = 1;
    output_params.device = Pa_GetDefaultOutputDevice();
    output_params.sampleFormat = paFloat32;
    output_params.suggestedLatency = suggested_latency;
    output_params.hostApiSpecificStreamInfo = NULL;

    std::cout << "Default input device: " << Pa_GetDefaultInputDevice() << "\n";
    std::cout << "Default output device: " << Pa_GetDefaultOutputDevice() << "\n";
}

int AudioEngine::start_stream()
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
        is_running = false;
        return error;
    }

    error = Pa_StartStream(stream);

    if (error != paNoError)
    {
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

        error = Pa_CloseStream( stream );
    }

    return error;
}
