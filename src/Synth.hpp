#ifndef SYNTH_HPP
#define SYNTH_HPP

#include "AudioBuffer.hpp"

#include "../lib/PortAudioCpp.hxx"

template <class T>
class Synth
{
public:
    Synth();

    Synth(AudioBuffer<T> buffer)
	: buffer(buffer)
    {
    }

    int process(const void *input, void *output,
		unsigned long frames_per_buffer,
		const PaStreamCallbackTimeInfo *time_info,
		PaStreamCallbackFlags status_flags)
    {
	T **out = static_cast<T**>(output);

	for (unsigned long i = 0; i < frames_per_buffer; ++i)
	{
	    /* Left Channel */
	    out[0][i] = buffer[index];
	    ++index;

	    if (index > buffer.size())
	    {
		return paComplete;
	    }
	    /* Right Channel */
	    // out[1][i] = buffer[index];
	    // ++index;
	}

	return paContinue;
    }

    T& operator[](unsigned long i) { return buffer[i]; }

    unsigned long get_index() { return index; }

private:
    AudioBuffer<T> buffer;
    unsigned long index = 0;

};

#endif
