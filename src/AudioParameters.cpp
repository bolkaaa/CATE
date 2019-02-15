#include "AudioParameters.hpp"

AudioParameters::AudioParameters(portaudio::System &system)
    : input_channels(2), output_channels(2), sample_rate(44100),
      frames_per_buffer(256)
{
    set(system);
}

AudioParameters::AudioParameters(portaudio::System &system, uint64_t sample_rate,
                                 uint32_t frames_per_buffer,
                                 uint8_t input_channels, uint8_t output_channels)
    : input_channels(input_channels), output_channels(output_channels),
      sample_rate(sample_rate), frames_per_buffer(frames_per_buffer)
{
    set(system);
}

void AudioParameters::set(portaudio::System &system)
{
    in = portaudio::DirectionSpecificStreamParameters(
        system.defaultInputDevice(),
        input_channels,
        portaudio::FLOAT32,
        false,
        system.defaultInputDevice().defaultLowInputLatency(),
        nullptr);

    out = portaudio::DirectionSpecificStreamParameters(
        system.defaultOutputDevice(),
        output_channels,
        portaudio::FLOAT32,
        false,
        system.defaultOutputDevice().defaultLowOutputLatency(),
        nullptr);

    strm = portaudio::StreamParameters(
        in,
        out,
        sample_rate,
        frames_per_buffer,
        paClipOff);
}
