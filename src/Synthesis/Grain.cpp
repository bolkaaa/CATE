#include <cstdio>

#include "Grain.hpp"
#include "Util.hpp"

namespace CATE {

Grain::Grain()
        : frames(0),
          remaining_frames(frames),
          active(false),
          env(frames, 0.0f),
          src(AudioBuffer(), frames)
{
}

Grain::Grain(int frames, float sustain, AudioBuffer buffer)
        : frames(frames),
          remaining_frames(frames),
          active(true),
          env(frames, sustain),
          src(buffer, frames)
{
}

float Grain::synthesize()
{
    --remaining_frames;
    float attack = 0.75;
    float release = 0.75;
    float sustain = 0.5;
    float amp = env.synthesize(attack, sustain, release);
    float sample = src.synthesize();
    return amp * sample;
}

bool Grain::is_active() const
{
    return (remaining_frames > 0);
}

} // CATE
