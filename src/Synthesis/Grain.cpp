#include <cstdio>

#include "Grain.hpp"

namespace CATE {

Grain::Grain()
        : remaining_samples(0),
          src(AudioBuffer()),
          env(env_params)
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
