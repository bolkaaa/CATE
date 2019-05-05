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

using std::cerr;

namespace CATE {

AudioEngine::AudioEngine(AudioSettings *audio_settings)
        : audio_settings(audio_settings),
          is_running(false)
{
    if (Pa_Initialize() != paNoError)
    {
        cerr << "PortAudio Error: " << Pa_GetErrorText(error) << "\n";
        return;
    }

    input_device = get_default_input_device();
    output_device = get_default_output_device();

    configure_stream_parameters();
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
                          audio_settings->get_sample_rate().value,
                          audio_settings->get_buffer_size().value,
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

void AudioEngine::configure_stream_parameters()
{
    input_parameters.device = input_device;
    input_parameters.channelCount = get_num_input_channels();
    input_parameters.sampleFormat = paFloat32;
    input_parameters.hostApiSpecificStreamInfo = nullptr;

    output_parameters.device = output_device;
    output_parameters.channelCount = get_num_output_channels();
    output_parameters.sampleFormat = paFloat32;
    output_parameters.hostApiSpecificStreamInfo = nullptr;
}

DeviceList AudioEngine::get_available_devices()
{
    auto num_devices = Pa_GetDeviceCount();
    auto device_list = DeviceList(num_devices);

    for (auto i = 0; i < num_devices; ++i)
    {
        const auto *device_info = Pa_GetDeviceInfo(i);
        device_list[i] = device_info->name;
    }

    return device_list;
}

const int AudioEngine::get_num_input_channels()
{
    const auto *info = Pa_GetDeviceInfo(input_device);
    return info->maxInputChannels;
}

const int AudioEngine::get_num_output_channels()
{
    const auto *info = Pa_GetDeviceInfo(output_device);
    return info->maxOutputChannels;
}

void AudioEngine::set_input_device(int selection_index)
{
    auto num_devices = Pa_GetDeviceCount();

    if (selection_index < 0 || selection_index > num_devices)
    {
        return;
    }

    input_device = selection_index;
}

void AudioEngine::set_output_device(int selection_index)
{
    auto num_devices = Pa_GetDeviceCount();

    if (selection_index < 0 || selection_index > num_devices)
    {
        return;
    }

    output_device = selection_index;
}

} // CATE
