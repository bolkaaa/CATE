#ifndef AUDIO_PARAMETERS_HPP
#define AUDIO_PARAMETERS_HPP

#include "portaudiocpp/PortAudioCpp.hxx"

class AudioParameters
{
public:
    /* Use defaults for audio system settings. */
    AudioParameters(portaudio::System &system);

    /* Use provided arguments for audio system settings. */
    AudioParameters(portaudio::System &system, uint64_t sample_rate,
                    uint32_t frames_per_buffer,
                    uint8_t input_channels, uint8_t output_channels);

    /* Get stream object. */
    portaudio::StreamParameters stream() { return strm; }

private:
    /* Set stream objects from parameters. */
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
