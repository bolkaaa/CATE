#ifndef AUDIO_PARAMETERS_HPP
#define AUDIO_PARAMETERS_HPP

#include "portaudiocpp/PortAudioCpp.hxx"

class AudioParameters
{
public:
    AudioParameters(portaudio::System &system);

    AudioParameters(portaudio::System &system, uint64_t sample_rate,
                    uint32_t frames_per_buffer,
                    uint8_t input_channels, uint8_t output_channels);

    portaudio::DirectionSpecificStreamParameters input() { return in; }

    portaudio::DirectionSpecificStreamParameters output() { return out; }

    portaudio::StreamParameters stream() { return strm; }

private:
    void set(portaudio::System &system);

    uint8_t input_channels;
    uint8_t output_channels;
    uint64_t sample_rate;
    uint32_t frames_per_buffer;
    portaudio::DirectionSpecificStreamParameters in;
    portaudio::DirectionSpecificStreamParameters out;
    portaudio::StreamParameters strm;
};

#endif
