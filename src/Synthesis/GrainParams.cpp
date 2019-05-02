
#include "GrainParams.hpp"

GrainParams::GrainParams()
    : max_grains(32),
      grain_size(1024),
      grain_density(100),
      grain_width(0.999)
{
}

int GrainParams::get_max_grains() const
{
    return max_grains;
}

int GrainParams::get_grain_size() const
{
    return grain_size;
}

float GrainParams::get_grain_density() const
{
    return grain_density;
}

float GrainParams::get_grain_width() const
{
    return grain_width;
}

void GrainParams::set_max_grains(int max_grains)
{
    GrainParams::max_grains = max_grains;
}

void GrainParams::set_grain_size(int grain_size)
{
    GrainParams::grain_size = grain_size;
}

void GrainParams::set_grain_density(float grain_density)
{
    GrainParams::grain_density = grain_density;
}

void GrainParams::set_grain_width(float grain_width)
{
    GrainParams::grain_width = grain_width;
}
