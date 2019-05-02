#include <iostream>
#include <cmath>

#include "Envelope.hpp"

namespace CATE {

Envelope::Envelope(const EnvelopeParams &params)
        : index(0),
          amp(0.0f),
          amp_incr(0.0f),
          params(params)
{
}

float Envelope::synthesize()
{
    auto attack_samples = static_cast<int>(params.get_sample_size() * params.get_attack());
    auto release_samples = static_cast<int>(params.get_sample_size() * params.get_release());

    if (index < attack_samples) // attack
    {
        amp_incr = params.get_sustain() / attack_samples;
    }
    else if (attack_samples < index && index < release_samples) // sustain
    {
        amp_incr = 0.0f;
    }
    else // release
    {
        amp_incr = -(params.get_sustain() / release_samples);
    }

    ++index;
    amp += amp_incr;

    return amp;
}


} // CATE
