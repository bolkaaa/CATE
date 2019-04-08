#include <cstdio>

#include "Grain.hpp"
#include "Util.hpp"

namespace CATE {

Grain::Grain()
        : samples(0),
          remaining_samples(samples),
          src(AudioBuffer(), samples),
          env(env_params)
{
}

Grain::Grain(int samples, const AudioBuffer &buffer, EnvelopeParams env_params)
        : samples(samples),
          remaining_samples(samples),
          src(buffer, samples),
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
