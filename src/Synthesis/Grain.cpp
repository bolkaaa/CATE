#include <cstdio>

#include "Grain.hpp"

namespace CATE {

Grain::Grain()
        : src(AudioBuffer()),
          env(env_params),
          remaining_samples(0)
{
}

Grain::Grain(const AudioBuffer &buffer, EnvelopeParams env_params)
        : remaining_samples(buffer.size()),
          src(buffer),
          env_params(env_params),
          env(env_params)
{
}

float Grain::synthesize()
{
    float sample = src.synthesize();
    float amp = env.synthesize();

    --remaining_samples;

    return amp * sample;
}

bool Grain::is_active() const
{
    return (remaining_samples > 0);
}


} // CATE
