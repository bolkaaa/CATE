#include "AudioParameters.hpp"

AudioParameters::AudioParameters(portaudio::System &system)
    : sr(44100), frames_per_buf(256), in_chan(2), out_chan(2)
{
    set(system);
}

AudioParameters::AudioParameters(portaudio::System &system, uint64_t sr,
                                 uint32_t frames_per_buf,
                                 uint8_t in_chan, uint8_t out_chan)
    : sr(sr), frames_per_buf(frames_per_buf), in_chan(in_chan), out_chan(out_chan)
{
    set(system);
}

void AudioParameters::set(portaudio::System &system)
{
    in = portaudio::DirectionSpecificStreamParameters(
        system.defaultInputDevice(),
        in_chan,
        portaudio::FLOAT32,
        false,
        system.defaultInputDevice().defaultLowInputLatency(),
        nullptr);

    out = portaudio::DirectionSpecificStreamParameters(
        system.defaultOutputDevice(),
        out_chan,
        portaudio::FLOAT32,
        false,
        system.defaultOutputDevice().defaultLowOutputLatency(),
        nullptr);

    strm = portaudio::StreamParameters(
        in,
        out,
        sr,
        frames_per_buf,
        paClipOff);
}
