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

#include "src/Util/NonCopyable.hpp"

using std::vector;

namespace CATE {

typedef vector<float> SampleRateVector;
typedef vector<unsigned long> BufferSizeVector;
typedef vector<int> BinSizeVector;
typedef vector<int> FrameSizeVector;

/* Functionality for storing audio settings used throughout the program, with possible settings for each
 * parameter stored as a fixed number of options.  */
class AudioSettings : NonCopyable
{
public:
    AudioSettings();

    inline unsigned long default_buffer_size() const { return available_buffer_sizes[default_buffer_size_index]; }
    inline float default_sample_rate() const { return available_sample_rates[default_sample_rate_index]; }
    inline int default_bin_size() const { return available_bin_sizes[default_bin_size_index]; }
    inline int default_frame_size() const { return available_bin_sizes[default_frame_size_index]; }

    inline unsigned long get_buffer_size() const { return buffer_size; }
    inline float get_sample_rate() const { return sample_rate; }
    inline int get_bin_size() const { return bin_size; }
    inline int get_frame_size() const { return frame_size; }

    inline const BufferSizeVector get_available_buffer_sizes() const { return available_buffer_sizes; }
    inline const SampleRateVector get_available_sample_rates() const { return available_sample_rates; }
    inline const BinSizeVector get_available_bin_sizes() const { return available_bin_sizes; }
    inline const FrameSizeVector get_available_frame_sizes() const { return available_frame_sizes; }
    inline const int get_default_buffer_size_index() const { return default_buffer_size_index; }
    inline const int get_default_sample_rate_index() const { return default_sample_rate_index; }
    inline const int get_default_bin_size_index() const { return default_bin_size_index; }
    inline const int get_default_frame_size_index() const { return default_frame_size_index; }
    void set_sample_rate(int selection_index);
    void set_buffer_size(int selection_index);
    void set_bin_size(int selection_index);
    void set_frame_size(int selection_index);

private:
    float sample_rate;
    unsigned long buffer_size;
    int bin_size;
    int frame_size;

    const int default_sample_rate_index = 0;
    const int default_buffer_size_index = 3;
    const int default_bin_size_index = 2;
    const int default_frame_size_index = 1;

    const SampleRateVector available_sample_rates = {44100.0f, 48000.0f, 88200.0f, 96000.0f};
    const BufferSizeVector available_buffer_sizes = {32, 64, 128, 256, 512, 1024, 2056, 4096};
    const BinSizeVector available_bin_sizes = {256, 512, 1024, 2056, 4096, 8192};
    const FrameSizeVector available_frame_sizes = {128, 256, 512, 1024};
};

} // CATE

#endif
