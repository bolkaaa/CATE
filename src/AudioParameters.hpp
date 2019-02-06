#ifndef AUDIO_PARAMETERS_HPP
#define AUDIO_PARAMETERS_HPP

#include "portaudiocpp/PortAudioCpp.hxx"

class AudioParameters
{
public:
    AudioParameters(portaudio::System &system);

    AudioParameters(portaudio::System &system, unsigned long sample_rate,
		    unsigned long frames_per_buffer,
		    unsigned int input_channels, unsigned int output_channels);

    portaudio::DirectionSpecificStreamParameters get_input() { return input; }

    portaudio::DirectionSpecificStreamParameters get_output() { return output; }

    portaudio::StreamParameters get_stream() { return stream; }

private:
    unsigned int input_channels;
    unsigned int output_channels;
    unsigned long sample_rate;
    unsigned long frames_per_buffer;
    portaudio::DirectionSpecificStreamParameters input;
    portaudio::DirectionSpecificStreamParameters output;
    portaudio::StreamParameters stream;
};

#endif
