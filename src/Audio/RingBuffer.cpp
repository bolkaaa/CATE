#include <algorithm>

#include "AudioBuffer.hpp"
#include "RingBuffer.hpp"

namespace CATE {

RingBuffer::RingBuffer(int size)
        : data(size),
          head(0), tail(0),
          high_water_mark(static_cast<int>(data.size() / 4))
{
    high_water_mark = static_cast<int>((high_water_mark > data.size()) ? data.size() : high_water_mark);
}

void RingBuffer::push(float elem)
{
    if (space_available())
    {
        data[tail] = elem;
        tail = static_cast<int>((tail + 1) % data.size());
    }
}

void RingBuffer::pop(float &ref)
{
    if (samples_available())
    {
        ref = data[head];
        head = static_cast<int>((head + 1) % data.size());
    }
}

int RingBuffer::samples_available()
{
    auto samples = static_cast<int>((tail - head + data.size()) % data.size());

    return samples;
}

int RingBuffer::space_available()
{
    int free = static_cast<int>((head - tail + data.size() - 1) % data.size());
    int undermark = high_water_mark - samples_available();
    auto space = std::min(undermark, free);

    return space;
}

} // CATE
