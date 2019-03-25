#include <cstdio>

#include "Grain.hpp"
#include "Util.hpp"

namespace CATE {

Grain::Grain()
        : dur(0.0f),
          sustain(0.0f),
          sample_rate(0.0f),
          active(false),
          sample_size(0),
          remaining_samples(0),
          env(dur, 10.0f, sustain, 10.0f, sample_rate),
          src(AudioBuffer(), dur, sample_rate)
{
}

Grain::Grain(float dur, float sustain, float sample_rate, AudioBuffer buffer)
        : dur(dur),
          sustain(sustain),
          sample_rate(sample_rate),
          active(true),
          sample_size(ms_to_samp(dur, sample_rate)),
          remaining_samples(sample_size),
          env(dur, 1.0f, sustain, 1.0f, sample_rate),
          src(buffer, dur, sample_rate)
{
}

float Grain::synthesize()
{
    if (!is_active())
    {
        return 0.0f;
    }

    --remaining_samples;
    float amp = env.synthesize();
    float sample = src.synthesize();

    return amp * sample;
}

bool Grain::is_active() const
{
    return (remaining_samples > 0);
}

} // CATE
