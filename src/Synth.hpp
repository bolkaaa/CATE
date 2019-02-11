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
    Synth<T>();

    Synth<T>(std::vector<AudioBuffer<T> > buffers);

    int process(const void *input, void *output,
		unsigned long frames_per_buffer,
		const PaStreamCallbackTimeInfo *time_info,
		PaStreamCallbackFlags status_flags);
    
private:
    std::vector<AudioBuffer<T> > buffers;
    uint16_t buffer_ix = 0;
    uint32_t buffer_pos = 0;
};

template <class T>
Synth<T>::Synth()
{
}

template <class T>
Synth<T>::Synth(std::vector<AudioBuffer<T> > buffers)
    : buffers(buffers)
{
}

template <class T>
int Synth<T>::process(const void *input, void *output,
		      unsigned long frames_per_buffer,
		      const PaStreamCallbackTimeInfo *time_info,
		      PaStreamCallbackFlags status_flags)
{
    const T **in = (const T**) (input);
    T **out = static_cast<T**> (output);
    uint32_t sz = buffers[buffer_ix].size();
    
    for (unsigned long i = 0; i < frames_per_buffer; i++) {
	/* Monophonic output. */
	out[0][i] = buffers[buffer_ix][buffer_pos];
	out[1][i] = buffers[buffer_ix][buffer_pos];
	++buffer_pos;
	
	if (buffer_pos > sz)
	{
	    /* Skipping through grains. */
	    buffer_ix = (buffer_ix + 4) % buffers.size();

	    /* Reset to beginning of next grain. */
	    buffer_pos -= sz;
	}
    }

    return paContinue;    

}

#endif
