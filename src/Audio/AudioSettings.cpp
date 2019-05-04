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

#include "AudioSettings.hpp"

#include <iostream>

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
