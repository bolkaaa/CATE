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
    /* Allocate memory for ring buffer. */
    explicit RingBuffer(int size);

    /* Insert element at head. */
    void push(float elem);

    /* Get tail element. */
    void pop(float &output);

    /* Calculate samples available in buffer. */
    int samples_available();

    /* Calculate space left in buffer. */
    int space_available();

private:
    vector<float> data;
    int head;
    int tail;
    int high_water_mark;
};

#endif
