#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

#include <algorithm>

#include "AudioBuffer.hpp"

/* Based on lock-free ring buffer implementation in "Audio Anecdotes Volume 2
 * (Greenebaum, Barzel)". Will be used if I do some concurrent audio processing
 * later on (not doing so at the moment). */

class RingBuffer : public AudioBuffer
{
public:
    /* Allocate memory for buffer (defaults to 1024 samples). */
    RingBuffer();

    /* Allocate and set parameters. */
    RingBuffer(uint32_t size);

    /* Insert element at head. */
    void push(float elem);

    /* Get tail element. */
    void pop(float &output);

    /* Calculate samples available in buffer. */
    uint32_t samples_available();

    /* Calculate space left in buffer. */
    uint32_t space_available();

private:
    vector<float> data;
    uint32_t head;
    uint32_t tail;
    uint32_t high_water_mark;
};

#endif
