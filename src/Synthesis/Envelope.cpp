#include "Envelope.hpp"
#include "Util.hpp"

namespace CATE {

Envelope::Envelope(float dur, float sustain, float sample_rate)
        : dur(dur),
          sustain(sustain),
          sample_rate(sample_rate),
          sample_index(0),
          sample_size(ms_to_samp(dur, sample_rate))
{
    rdur = 1.0f / sample_size;
    rdur2 = rdur * rdur;
    curve = -8.0f * sustain * (rdur2);
    reset();
}

void Envelope::reset()
{
    amp = 0.0f;
    slope = 4.0f * sustain * (rdur - (rdur2));
}

float Envelope::synthesize()
{
    amp += slope;
    slope += curve;
    return amp;
}


} // CATE
