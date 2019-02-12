#ifndef SYNTH_HPP
#define SYNTH_HPP

#include <cmath>

#include "AudioBuffer.hpp"
#include "Database.hpp"
#include "RingBuffer.hpp"

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

    /* Temporary function to test ring buffer. */
    void ring_buffer_test(T &ref) { ring_buffer.pop(ref); }

    /* Data member accessible from other thread. */
    T shared = 0;
    
private:
    std::vector<AudioBuffer<T> > buffers;
    RingBuffer<T> ring_buffer;
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
    (void) status_flags;
    (void) time_info;
    
    T **out = static_cast<T**> (output);
    T **in = (T**) (input);
    uint32_t sz = buffers[buffer_ix].size();
    
    for (unsigned long i = 0; i < frames_per_buffer; i++)
    {
	/* Testing simple audio file grain playback. */
	T test_out = buffers[buffer_ix][buffer_pos];
	out[0][i] = test_out;
	out[1][i] = test_out;
	++buffer_pos;

	/* Pushing data from microphone into ring buffer, popping to
	   shared data member to print value. Will mostly be useful
	   later when real-time manipulation of input stream is
	   needed. */
	ring_buffer.push(**in);
	ring_buffer.pop(shared);

	if (buffer_pos > sz)
	{
	    /* Testing skipping through grains (works better with longer 
	       buffers.) */
	    uint8_t grain_skip = 4;
	    buffer_ix = (buffer_ix + grain_skip) % buffers.size();

	    /* Reset to beginning of next grain. */
	    buffer_pos -= sz;
	}
    }

    return paContinue;    

}

#endif
