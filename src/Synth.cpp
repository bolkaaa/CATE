#include "Synth.hpp"

Synth::Synth()
    : fft_bin_size(1024),
      audio_buffer_size(256)
{
}

Synth::Synth(uint16_t fft_bin_size, uint16_t audio_buffer_size)
    : fft_bin_size(fft_bin_size),
      audio_buffer_size(audio_buffer_size)
{
}

