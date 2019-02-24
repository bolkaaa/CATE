#include "Synth.hpp"

Synth::Synth(uint16_t fft_bin_size, uint16_t audio_buffer_size)
    : fft_bin_size(fft_bin_size),
      audio_buffer_size(audio_buffer_size)
{
    fft_data = new FFT(fft_bin_size);
}

Synth::~Synth()
{
    delete fft_data;
}

