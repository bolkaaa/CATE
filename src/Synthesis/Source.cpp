#include "Source.hpp"
#include "Util.hpp"

namespace CATE {

Source::Source(const AudioBuffer &buffer, int num_samples)
        : buffer(buffer),
          num_samples(num_samples),
          index(0)
{
}

float Source::synthesize()
{
    if (index > num_samples)
    {
        return 0.0f;
    }

    float sample = buffer[index];
    ++index;
    return sample;
}

} // CATE
