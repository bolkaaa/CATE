#ifndef SYNTH_HPP
#define SYNTH_HPP

#include "AudioBuffer.hpp"

#include "../lib/PortAudioCpp.hxx"

class Synth
{
public:
    Synth();

    Synth(AudioBuffer buffer);

    int process(const void *input, void *output,
		unsigned long frames_per_buffer,
		const PaStreamCallbackTimeInfo *time_info,
		PaStreamCallbackFlags status_flags);

private:
    AudioBuffer buffer;
    unsigned long index;

};

#endif
