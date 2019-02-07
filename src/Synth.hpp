#ifndef SYNTH_HPP
#define SYNTH_HPP

#include <cmath>

#include "AudioBuffer.hpp"
#include "Database.hpp"

#include "portaudio.h"

template <class T>
class Synth
{
public:
    Synth();

    Synth(std::vector<AudioBuffer<T>> buffers, std::string file)
	: buffers(buffers), file(file), buffer_index(0), buffer_pos(0)
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
	    T mix = buffers[buffer_index][buffer_pos];

	    /* Monophonic */
	    out[0][i] = mix;
	    out[1][i] = mix;

	    ++buffer_pos;

	    /* Exit loop when EOF reached. */
	    if (buffer_pos > buffers[buffer_index].size())
	    {
		buffer_pos = 0;
	    }
	}

	return paContinue;
    }

private:
    std::vector<AudioBuffer<T>> buffers;
    std::string file;
    unsigned int buffer_index;
    unsigned long buffer_pos;
    
    

};

#endif
