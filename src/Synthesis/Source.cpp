#include "Source.hpp"
#include "Util.hpp"

namespace CATE {

Source::Source(AudioBuffer buffer, int frames)
        : buffer(buffer),
          frames(frames),
          index(0)
{
}

float Source::synthesize()
{
    float sample = buffer[index];
    ++index;
    return sample;
}

} // CATE
