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
#include "src/Util/Param.hpp"

using std::vector;

namespace CATE {

typedef vector<float> SampleRateVector;
typedef vector<unsigned long> BufferSizeVector;
typedef vector<int> BinSizeVector;

/* Functionality for storing audio settings used throughout the program, with possible settings for each
 * parameter stored as a fixed number of options.  */
class AudioSettings : NonCopyable
{
public:
    AudioSettings();

    /* Get parameter objects. */
    inline const FixedParam<float> get_sample_rate() const { return sample_rate; }
    inline const FixedParam<unsigned long> get_buffer_size() const { return buffer_size; }
    inline const FixedParam<int> get_bin_size() const { return bin_size; }

    /* Set parameter values. */
    void set_sample_rate(int selection_index) { sample_rate.set(selection_index); }
    void set_buffer_size(int selection_index) { buffer_size.set(selection_index); }
    void set_bin_size(int selection_index) { bin_size.set(selection_index); }

private:
    FixedParam<float> sample_rate;
    FixedParam<unsigned long> buffer_size;
    FixedParam<int> bin_size;
};

} // CATE

#endif
