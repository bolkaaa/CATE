#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

#include <iostream>
#include <algorithm>

/* Based on lock-free ring buffer implementation in "Audio Anecdotes Volume 2 
   (Greenebaum, Barzel)". */

template <class T>
class RingBuffer
{
public:
    /* Allocate memory for buffer. */
    RingBuffer<T>();

    /* Allocate and set parameters. */
    RingBuffer<T>(uint32_t sz, uint32_t high_water_mark);

    /* Delete data pointer. */
    ~RingBuffer<T>();

    /* Insert element at head. */
    void push(T elem);

    /* Get tail element. */
    void pop(T &ref);

    /* Calculate samples available in buffer. */
    uint32_t samples_available();

    /* Calculate space left in buffer. */
    uint32_t space_available();

private:
    T *data;
    uint32_t head;
    uint32_t tail;
    uint32_t high_water_mark;
    const uint32_t sz;
};

template <class T>
RingBuffer<T>::RingBuffer()
    : high_water_mark(1024), sz(4096)
{
    data = new T[sz];
    high_water_mark = (high_water_mark > sz) ? sz : high_water_mark;
}

template <class T>
RingBuffer<T>::~RingBuffer()
{
    delete data;
}

template <class T>
RingBuffer<T>::RingBuffer(uint32_t sz, uint32_t high_water_mark)
    : sz(sz), head(0), tail(0), high_water_mark(high_water_mark)
{
    data = new T[sz];

    for (uint32_t i = 0; i < sz; ++i)
    {
	data[i] = 0;
    }

    high_water_mark = (high_water_mark > sz) ? sz : high_water_mark;
}

template <class T>
void RingBuffer<T>::push(T elem)
{
    if (space_available())
    {
	data[tail] = elem;
	tail = (tail + 1) % sz;
    }
}

template <class T>
void RingBuffer<T>::pop(T &ref)
{
    if (samples_available())
    {
	ref = data[head];
	head = (head + 1) % sz;
    }
}

template <class T>
uint32_t RingBuffer<T>::samples_available()
{
    uint32_t samples = (tail - head + sz) % sz;

    return samples;
}

template <class T>
uint32_t RingBuffer<T>::space_available()
{
    uint32_t free = (head - tail + sz - 1) % sz;
    uint32_t undermark = high_water_mark - samples_available();
    uint32_t space = std::min(undermark, free);

    return space;
}

#endif
