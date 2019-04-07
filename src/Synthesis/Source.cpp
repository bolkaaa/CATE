#include "Source.hpp"
#include "Util.hpp"

namespace CATE {

Source::Source(AudioBuffer buffer, int frames)
        : buffer(buffer),
          frames(frames),
          index(0),
          pitch(0.5)
{
}

float Source::synthesize()
{
    float sample = cubic_interpolation();
    ++index;
    return sample;
}

float Source::cubic_interpolation()
{
    float x0 = buffer[index];
    float x1 = buffer[index+1];
    float x2 = buffer[index+2];
    float x3 = buffer[index+3];

    float a0 = x3 - x2 - x0 + x1;
    float a1 = x0 - x1 - a0;
    float a2 = x2 - x0;
    float a3 = x1;

    return (a0 * (pitch * pitch * pitch)) + (a1 * (pitch * pitch)) + (a2 * pitch) + (a3);
}

} // CATE
