#include "AudioSettings.hpp"

namespace CATE {

AudioSettings::AudioSettings()
{
    buffer_size = default_buffer_size();
    sample_rate = default_sample_rate();
    bin_size = default_bin_size();
}

void AudioSettings::set_buffer_size(int selection_index)
{
    if (selection_index < 0 || selection_index > available_buffer_sizes.size())
    {
        return;
    }

    buffer_size = available_buffer_sizes[selection_index];
}

void AudioSettings::set_bin_size(int selection_index)
{
    if (selection_index < 0 || selection_index > available_bin_sizes.size())
    {
        return;
    }

    bin_size = available_bin_sizes[selection_index];
}

void AudioSettings::set_sample_rate(int selection_index)
{
    if (selection_index < 0 || selection_index > available_sample_rates.size())
    {
        return;
    }

    sample_rate = available_sample_rates[selection_index];
}

} // CATE
