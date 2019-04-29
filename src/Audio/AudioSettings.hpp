/*
  CATE: Concatenative Audio Processing Application
  Copyright (c) 2019 Liam Wyllie. All rights reserved.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#ifndef AUDIOSETTINGS_HPP
#define AUDIOSETTINGS_HPP

#include <array>

namespace CATE {

/* Functionality for storing audio settings used throughout the program. */
class AudioSettings
{
public:
    AudioSettings();

    /* Get default buffer size from fixed list of buffer sizes. */
    unsigned long default_buffer_size() const { return buffer_size_list[4]; }

    /* Get default sample rate from fixed list of sample rates. */
    float default_sample_rate() const { return sample_rate_list[0]; }

    /* Get default FFT bin size from fixed list of bin sizes. */
    int default_bin_size() const { return bin_size_list[2]; }

    /* Get current buffer size. */
    unsigned long get_buffer_size() const { return buffer_size; }

    /* Get current sample rate. */
    float get_sample_rate() const { return sample_rate; }

    /* Get current input device. */
    int get_input_device() const { return input_device; }

    /* Get current output device. */
    int get_output_device() const { return output_device; }

    /* Get number of input channels. */
    int get_num_input_channels() const { return num_input_channels; }

    /* Get number of output channels. */
    int get_num_output_channels() const { return num_output_channels; }

    /* Get current FFT bin size. */
    int get_bin_size() const { return bin_size; }

    /* Set current buffer size to new value. */
    void set_buffer_size(unsigned long new_buffer_size) { buffer_size = new_buffer_size; }

    /* Set current sample rate to new value. */
    void set_sample_rate(float new_sample_rate) { sample_rate = new_sample_rate; }

    /* Set current input device index to new value. */
    void set_input_device(int new_input_device) { input_device = new_input_device; }

    /* Set current output device index to new value. */
    void set_output_device(int new_output_device) { output_device = new_output_device; }

    /* set current number of input channels to new value. */
    void set_num_input_channels(int new_num_input_channels) { num_input_channels = new_num_input_channels; }

    /* set current number of output channels to new value. */
    void set_num_output_channels(int new_num_output_channels) { num_output_channels = new_num_output_channels; }

    /* Set current FFT bin size to new value. */
    void set_bin_size(int new_bin_size) { bin_size = new_bin_size; }

private:
    unsigned long buffer_size;
    float sample_rate;
    int bin_size;
    int input_device;
    int output_device;
    int num_input_channels;
    int num_output_channels;
    const std::array<unsigned long, 8> buffer_size_list = {32, 64, 128, 256, 512, 1024, 2056, 4096};
    const std::array<float, 4> sample_rate_list = {44100.0f, 48000.0f, 88200.0f, 96000.0f};
    const std::array<int, 8> bin_size_list = {256, 512, 1024, 2056, 4096, 8192};
};

} // CATE

#endif
