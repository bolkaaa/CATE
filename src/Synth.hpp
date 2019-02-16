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
    /* Instantiate by setting size in samples for audio processing buffers. */
    Synth<T>(uint16_t buffer_size);

    /* Main audio processing function to be called from outside class. */
    int process(const void *input, void *output,
                unsigned long frames_per_buffer,
                const PaStreamCallbackTimeInfo *time_info,
                PaStreamCallbackFlags status_flags);

private:
    /* Provide data from audio input to ring buffer and pop to processing buffer. */
    void prepare_buffers(T **in);

    /* Placeholder function... */
    void analyse();

    uint16_t buffer_size = 4096;
    RingBuffer<T> input_buffer;
    T *process_buffer;
};

template <class T>
Synth<T>::Synth(uint16_t buffer_size)
    : buffer_size(buffer_size), input_buffer(buffer_size, buffer_size/4),
      process_buffer(new T[buffer_size])
{
}

template <class T>
void Synth<T>::prepare_buffers(T **in)
{
    /* Get data from input into ring buffer. */
    for (uint16_t i = 0; i < buffer_size; ++i)
    {
        input_buffer.push(in[0][i]);
    }

    /* Pop ring buffer data into process buffer. */
    for (uint16_t i = 0; i < buffer_size; ++i)
    {
        input_buffer.pop(process_buffer[i]);
    }
}

template <class T>
void Synth<T>::analyse()
{
    for (uint16_t i = 0; i < buffer_size; ++i)
    {
        // ...
    }
}

template <class T>
int Synth<T>::process(const void *input, void *output,
                      unsigned long frames_per_buffer,
                      const PaStreamCallbackTimeInfo *time_info,
                      PaStreamCallbackFlags status_flags)
{
    (void) status_flags;
    (void) time_info;
    T **out = (T**) (output);
    T **in = (T**) (input);

    /* Fill buffers from audio input. */
    prepare_buffers(in);

    /* Perform processing. */
    analyse();

    /* Main processing block. */
    for (uint32_t i = 0; i < frames_per_buffer; i++)
    {
        out[0][i] = process_buffer[i];
        out[1][i] = process_buffer[i];
    }

    return paContinue;
}

#endif
