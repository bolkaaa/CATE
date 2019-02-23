#include <algorithm>

#include "AudioBuffer.hpp"
#include "RingBuffer.hpp"

RingBuffer::RingBuffer()
    : AudioBuffer(1024), data(AudioBuffer::data),
      head(0), tail(0),
      high_water_mark(data.size() / 4)
{
    high_water_mark = (high_water_mark > data.size()) ? data.size() : high_water_mark;
}

RingBuffer::RingBuffer(uint32_t size)
    : AudioBuffer(size), data(AudioBuffer::data),
      head(0), tail(0),
      high_water_mark(data.size() / 4)
{
    high_water_mark = (high_water_mark > data.size()) ? data.size() : high_water_mark;
}

void RingBuffer::push(float elem)
{
    if (space_available())
    {
        data[tail] = elem;
        tail = (tail + 1) % data.size();
    }
}

void RingBuffer::pop(float &ref)
{
    if (samples_available())
    {
        ref = data[head];
        head = (head + 1) % data.size();
    }
}

uint32_t RingBuffer::samples_available()
{
    uint32_t samples = (tail - head + data.size()) % data.size();

    return samples;
}

uint32_t RingBuffer::space_available()
{
    uint32_t free = (head - tail + data.size() - 1) % data.size();
    uint32_t undermark = high_water_mark - samples_available();
    uint32_t space = std::min(undermark, free);

    return space;
}
