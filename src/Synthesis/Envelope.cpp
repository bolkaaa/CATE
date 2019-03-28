#include <iostream>
#include <cmath>

#include "Envelope.hpp"
#include "Util.hpp"

namespace CATE {

Envelope::Envelope(int samples, float sustain)
        : samples(samples),
          sustain(sustain),
          amp(0),
          amp_incr(0),
          index(0)
{
}

float Envelope::synthesize(float attack, float sustain, float release)
{
    int attack_samples = samples * attack;
    int release_samples = samples * release;

    if (index < attack_samples)
    {
        amp_incr = sustain / attack_samples;
    }
    else if (attack_samples < index && index < release_samples)
    {
        amp_incr = 0.0f;
    }
    else
    {
        amp_incr = -(sustain / release_samples);
    }

    ++index;
    amp += amp_incr;

    return amp;
}

} // CATE
