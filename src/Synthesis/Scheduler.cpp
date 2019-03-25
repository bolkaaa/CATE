#include <random>
#include <cmath>

#include "Scheduler.hpp"
#include "Util.hpp"

namespace CATE {

Scheduler::Scheduler(map<string, AudioFile> &files, float sample_rate)
        : files(files),
          grain_density(2.0f),
          sample_rate(sample_rate),
          buffer(AudioBuffer(buffer_size)),
          next_onset(0),
          grain_index(0),
          gen(seed()),
          dist(uniform_real_distribution<float>(0.0f, 1.0f))
{
}

void Scheduler::create_grain(int marker, string filename)
{
    grain_size = samp_to_ms(buffer_size, sample_rate);

    for (int i = 0; i < buffer_size; ++i)
    {
        int buffer_index = (i + marker) % files[filename].data.size();
        buffer[i] = files[filename].data[buffer_index];
    }

    for (auto &grain : grains)
    {
        if (!grain.is_active())
        {
            grain = Grain(grain_size, 1.0f / max_grains, sample_rate, buffer);
            return;
        }
    }
}

float Scheduler::schedule(int marker, string filename)
{
    if (next_onset == 0)
    {
        create_grain(marker, filename);
        next_onset += get_next_inter_onset();
    }

    --next_onset;

    return synthesize_grains();
}

float Scheduler::synthesize_grains()
{
    float output = 0.0f;

    for (auto &grain : grains)
    {
        if (grain.is_active())
        {
            output += grain.synthesize();
        }
    }

    return output;
}

int Scheduler::get_next_inter_onset()
{
    float r = dist(gen);
    float min = 128;
    float max = 128;
    auto inter_onset = static_cast<int>(min + (r * (max - min)));
    return inter_onset;
}

} // CATE
