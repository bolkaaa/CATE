#include <random>
#include <cmath>

#include "Scheduler.hpp"

namespace CATE {

Scheduler::Scheduler(const map<string, AudioFile> &files, float sample_rate)
        : files(files),
          sample_rate(sample_rate),
          buffer(AudioBuffer(grain_size)),
          grains(vector<Grain>(max_grains)),
          env_params(EnvelopeParams{0.5f, 0.5f, 0.5f, grain_size}),
          next_onset(0),
          grain_index(0),
          gen(seed()),
          grain_density(32),
          grain_width(0.1),
          dist(uniform_real_distribution<float>(0.0f, 1.0f))
{
}


void Scheduler::fill_buffer(int marker, const string &file_name)
{
    for (int i = 0; i < grain_size; ++i)
    {
        int file_pos = i + marker;
        buffer[i] = files[file_name].data[file_pos];
    }
}

void Scheduler::create_grain(int marker, const string &file_name)
{
    fill_buffer(marker, file_name);

    for (auto &grain : grains)
    {
        if (!grain.is_active())
        {
            grain = Grain(buffer, env_params);
            return;
        }
    }
}

float Scheduler::schedule(int marker, const string &filename)
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
    float random_value = dist(gen);

    auto avg_onset = static_cast<int>(sample_rate / grain_density);
    auto min_onset = static_cast<int>(avg_onset - (avg_onset * grain_width));
    auto max_onset = static_cast<int>(avg_onset + (avg_onset * grain_width));
    auto inter_onset = static_cast<int>(1 + (min_onset + (random_value * (max_onset - min_onset))));

    return inter_onset;
}

void Scheduler::set_grain_attack(float new_grain_attack)
{
    env_params.attack = new_grain_attack;
}

void Scheduler::set_grain_release(float new_grain_release)
{
    env_params.release = new_grain_release;
}

void Scheduler::set_grain_density(int new_grain_density)
{
    grain_density = new_grain_density;
}

void Scheduler::set_grain_width(int new_grain_width)
{
    grain_width = new_grain_width;
}

void Scheduler::set_grain_size(int new_grain_size)
{
    grain_size = new_grain_size;
}

void Scheduler::load_files(const map<string, AudioFile> &new_files)
{
    files = new_files;
}

} // CATE
