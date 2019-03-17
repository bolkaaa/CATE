#include <cstdio>

#include "Grain.hpp"

namespace CATE {

Grain::Grain()
        : dur(0.0f),
          sustain(0.0f),
          sample_rate(0.0f),
          env(dur, sustain, sample_rate),
          active(false),
          src(AudioFile(), dur, sample_rate)
{
}

Grain::Grain(float dur, float sustain, float sample_rate, const AudioFile &segment)
        : dur(dur),
          sustain(sustain),
          active(false),
          env(dur, sustain, sample_rate),
          src(segment, dur, sample_rate)
{
}

float Grain::synthesize()
{
    float amp = env.synthesize();
    float sample = src.synthesize();
    return amp * sample;
}

bool Grain::is_active() const
{
    return active;
}

void Grain::activate()
{
    active = true;
    env.reset();
    src.reset();
}

} // CATE
