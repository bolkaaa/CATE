#include "Source.hpp"

namespace CATE {

Source::Source(const AudioBuffer &buffer)
        : buffer(buffer),
          index(0)
{
}

float Source::synthesize()
{
    if (index > buffer.size())
    {
        return 0.0f;
    }

    float sample = buffer[index];
    ++index;
    return sample;
}

} // CATE
