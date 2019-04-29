#include "AudioSettings.hpp"

namespace CATE {

AudioSettings::AudioSettings()
        : input_device(0),
          output_device(0),
          num_input_channels(2),
          num_output_channels(2)
{
    buffer_size = default_buffer_size();
    sample_rate = default_sample_rate();
    bin_size = default_bin_size();
}

} // CATE
