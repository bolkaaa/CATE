#include <random>
#include <cmath>

#include "Scheduler.hpp"
#include "Util.hpp"

namespace CATE {

Scheduler::Scheduler(vector<AudioFile> &files, float sample_rate)
        : files(files),
          max_onset(files[0].data.size()),
          grain_density(1.0f),
          sample_rate(sample_rate),
          next_onset(0),
          grain_index(0),
          max_grains(16),
          gen(seed()),
          dist(uniform_real_distribution<float>(0.0f, 1.0f))
{
    int start = 0;

    for (int i = start; i < start + max_grains; ++i)
    {
        files[i].split(0, 1024);
        float dur = samp_to_ms(1024, sample_rate);
        float sustain = 1.0f / max_grains;
        grains.emplace_back(Grain(dur, sustain, sample_rate, files[i]));

    }
}

float Scheduler::schedule()
{
    if (next_onset == 0)
    {
        next_onset += get_next_inter_onset();
        grain_index = (grain_index + 1) % max_grains;
        grains[grain_index].activate();
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
    int min_onset = 1;
    int max_onset = static_cast<int>(50 + (r * 500));
    auto inter_onset = static_cast<int>(min_onset + (r * (max_onset - min_onset)));
    return inter_onset;
}


} // CATE
