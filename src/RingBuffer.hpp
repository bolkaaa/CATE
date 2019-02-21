#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

#include <algorithm>

#include "AudioBuffer.hpp"

/* Based on lock-free ring buffer implementation in "Audio Anecdotes Volume 2
   (Greenebaum, Barzel)". */

template <class T>
class RingBuffer : public AudioBuffer<T>
{
public:
    /* Allocate memory for buffer (defaults to 1024 samples). */
    RingBuffer<T>();

    /* Allocate and set parameters. */
    RingBuffer<T>(uint32_t sz);

    /* Insert element at head. */
    void push(T elem);

    /* Get tail element. */
    void pop(T &ref);

    /* Calculate samples available in buffer. */
    uint32_t samples_available();

    /* Calculate space left in buffer. */
    uint32_t space_available();

private:
    vector<T> data;
    uint32_t head;
    uint32_t tail;
    uint32_t high_water_mark;
};

template <class T>
RingBuffer<T>::RingBuffer()
    : AudioBuffer<T>(1024), data(AudioBuffer<T>::data),
      head(0), tail(0),
      high_water_mark(data.size() / 4)
{
    high_water_mark = (high_water_mark > data.size()) ? data.size() : high_water_mark;
}

template <class T>
RingBuffer<T>::RingBuffer(uint32_t sz)
    : AudioBuffer<T>(sz), data(AudioBuffer<T>::data),
      head(0), tail(0),
      high_water_mark(data.size() / 4)
{
    high_water_mark = (high_water_mark > data.size()) ? data.size() : high_water_mark;
}

template <class T>
void RingBuffer<T>::push(T elem)
{
    if (space_available())
    {
        data[tail] = elem;
        tail = (tail + 1) % data.size();
    }
}

template <class T>
void RingBuffer<T>::pop(T &ref)
{
    if (samples_available())
    {
        ref = data[head];
        head = (head + 1) % data.size();
    }
}

template <class T>
uint32_t RingBuffer<T>::samples_available()
{
    uint32_t samples = (tail - head + data.size()) % data.size();

    return samples;
}

template <class T>
uint32_t RingBuffer<T>::space_available()
{
    uint32_t free = (head - tail + data.size() - 1) % data.size();
    uint32_t undermark = high_water_mark - samples_available();
    uint32_t space = std::min(undermark, free);

    return space;
}

#endif
