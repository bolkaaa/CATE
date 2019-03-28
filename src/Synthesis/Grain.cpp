#include <cstdio>

#include "Grain.hpp"
#include "Util.hpp"

namespace CATE {

Grain::Grain()
        : frames(0),
          remaining_frames(frames),
          src(AudioBuffer(), frames),
          env(env_params)
{
}

Grain::Grain(int frames, AudioBuffer buffer, EnvelopeParams env_params)
        : frames(frames),
          remaining_frames(frames),
          src(buffer, frames),
          env_params(env_params),
          env(env_params)
{
}

float Grain::synthesize()
{
    --remaining_frames;
    float amp = env.synthesize();
    float sample = src.synthesize();
    return amp * sample;
}

bool Grain::is_active() const
{
    return (remaining_frames > 0);
}

} // CATE
