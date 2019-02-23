#include "AudioEngine.hpp"

#include <cstdlib>
#include <cstdio>
#include <iostream>

AudioEngine::AudioEngine()
{
    is_running = false;
    init();
}

AudioEngine::~AudioEngine()
{
    error = Pa_Terminate();

    if (error != paNoError)
    {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(error) << "\n";
    }
}


int AudioEngine::processing_callback(const void *input_buffer,
                                     void *output_buffer,
                                     unsigned long frames_per_buffer,
                                     const PaStreamCallbackTimeInfo* time_info,
                                     PaStreamCallbackFlags status_flags)
{
    (void) input_buffer;
    (void) output_buffer;
    (void) frames_per_buffer;
    (void) time_info;
    (void) status_flags;

    return 0;
}

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

    if (output_params.device > 0)
    {
        sample_rate = Pa_GetDeviceInfo(output_params.device)->defaultSampleRate;
    }
    else
    {
        sample_rate = 48000.0;
    }

    frames_per_buffer = 1024;

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
