#include "Source.hpp"
#include "Util.hpp"

namespace CATE {

Source::Source(const AudioFile &segment, float dur, float sample_rate)
        : segment(segment),
          dur(dur),
          sample_rate(sample_rate),
          sample_index(0),
          active(false)
{
    sample_size = ms_to_samp(dur, sample_rate);

    if (sample_size > segment.data.size())
    {
        sample_size = segment.data.size();
    }
}

float Source::synthesize()
{
    /* TODO: Interpolation, positional control etc. */
    float sample = segment.data[sample_index];

    /* Messing around with linear pitch increase. */
    sample_index += 4;

    if (sample_index > segment.data.size())
    {
        active = false;
    }

    return (sample_index > segment.data.size()) ? 0.0f : sample;
}

void Source::reset()
{
    sample_index = 0;
}

} // CATE
