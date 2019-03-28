#include <random>
#include <cmath>

#include "Scheduler.hpp"
#include "Util.hpp"

namespace CATE {

Scheduler::Scheduler(map<string, AudioFile> &files, float sample_rate)
        : files(files),
          sample_rate(sample_rate),
          buffer(AudioBuffer(buffer_size)),
          grain_size(buffer_size),
          next_onset(0),
          grain_index(0),
          gen(seed()),
          dist(uniform_real_distribution<float>(0.0f, 1.0f))
{
}

void Scheduler::create_grain(int marker, string filename)
{
    for (int i = 0; i < buffer_size; ++i)
    {
        int file_pos = (i + marker);

        if (file_pos < files[filename].data.size())
        {
            buffer[i] = files[filename].data[file_pos];
        }
        else
        {
            buffer[i] = 0.0f;
        }
    }

    for (auto &grain : grains)
    {
        if (!grain.is_active())
        {
            float amp = dist(gen);
            grain = Grain(grain_size, amp, buffer);
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
    float density = 16;
    int avg_onset = sample_rate / density;
    float spacing = 0.25;
    int min_onset = avg_onset - (avg_onset * spacing);
    int max_onset = avg_onset + (avg_onset * spacing);
    auto inter_onset = min_onset + (r * (max_onset - min_onset));
    return inter_onset;
}

} // CATE
