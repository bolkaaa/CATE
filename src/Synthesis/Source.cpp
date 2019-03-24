#include "Source.hpp"
#include "Util.hpp"

namespace CATE {

Source::Source(AudioBuffer buffer, float dur, float sample_rate)
        : buffer(buffer),
          dur(dur),
          sample_rate(sample_rate),
          sample_index(0),
          active(false)
{
    sample_size = ms_to_samp(dur, sample_rate);
}

float Source::synthesize()
{
    float sample = buffer[sample_index];
    sample_index = (sample_index + 1) % sample_size;
    return sample;
}

void Source::reset()
{
    sample_index = 0;
}

} // CATE
