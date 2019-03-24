#include <iostream>
#include <cmath>

#include "Envelope.hpp"
#include "Util.hpp"

namespace CATE {

Envelope::Envelope(float dur, float attack, float sustain, float release, float sample_rate)
        : dur(dur),
          attack(attack),
          sustain(sustain),
          release(release),
          sample_rate(sample_rate),
          sample_size(ms_to_samp(dur, sample_rate)),
          index(0)
{
}

float Envelope::synthesize()
{
    auto amp = static_cast<float>(0.5 * (1 - std::cos((2 * M_PI * index) / (sample_size - 1))));
    return amp;
}

} // CATE
