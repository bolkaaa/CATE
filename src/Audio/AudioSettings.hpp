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

#include <vector>

using std::vector;

namespace CATE {

/* Functionality for storing audio settings used throughout the program, with possible settings for each
 * parameter stored as a fixed number of options.  */
class AudioSettings
{
public:
    AudioSettings();

    /* Deleted copy constructor. */
    AudioSettings(const AudioSettings &rhs) = delete;

    /* Deleted copy assignment operator. */
    void operator=(AudioSettings &rhs) = delete;

    /* Get default buffer size from fixed list of buffer sizes. */
    unsigned long default_buffer_size() const { return available_buffer_sizes[4]; }

    /* Get default sample rate from fixed list of sample rates. */
    float default_sample_rate() const { return available_sample_rates[0]; }

    /* Get default FFT bin size from fixed list of bin sizes. */
    int default_bin_size() const { return available_bin_sizes[2]; }

    /* Get current buffer size. */
    unsigned long get_buffer_size() const { return buffer_size; }

    /* Get current sample rate. */
    float get_sample_rate() const { return sample_rate; }

    /* Get current FFT bin size. */
    int get_bin_size() const { return bin_size; }

    /* Get list of available default buffer sizes. */
    const vector<unsigned long> get_available_buffer_sizes() const { return available_buffer_sizes; }

    /* Get list of available default sample rates. */
    const vector<float> get_available_sample_rates() const { return available_sample_rates; }

    /* Get list of available default bin sizes. */
    const vector<int> get_available_bin_sizes() const { return available_bin_sizes; }

    /* Get default buffer size index. */
    const int get_default_buffer_size_index() const { return default_buffer_size_index; }

    /* Get default sample rate index. */
    const int get_default_sample_rate_index() const { return default_sample_rate_index; }

    /* Get default bin size index. */
    const int get_default_bin_size_index() const { return default_bin_size_index; }

    /* Set buffer size through index of default buffer sizes. */
    void set_buffer_size(int selection_index);

    /* Set bin size through index of default bin sizes. */
    void set_bin_size(int selection_index);

    /* Set sample rate through index of default sample rates. */
    void set_sample_rate(int selection_index);

private:
    unsigned long buffer_size;
    float sample_rate;
    int bin_size;
    const int default_buffer_size_index = 3;
    const int default_sample_rate_index = 0;
    const int default_bin_size_index = 2;
    const vector<unsigned long> available_buffer_sizes = {32, 64, 128, 256, 512, 1024, 2056, 4096};
    const vector<float> available_sample_rates = {44100.0f, 48000.0f, 88200.0f, 96000.0f};
    const vector<int> available_bin_sizes = {256, 512, 1024, 2056, 4096, 8192};
};

} // CATE

#endif
