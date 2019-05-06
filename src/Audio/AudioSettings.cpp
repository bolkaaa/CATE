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

namespace CATE {

AudioSettings::AudioSettings()
    : sample_rate({44100.0f, 48000.0f, 88200.0f, 96000.0f}, 0),
      buffer_size({32, 64, 128, 256, 512, 1024, 2056, 4096}, 5),
      bin_size({256, 512, 1024, 2056, 4096, 8192}, 2)
{
}

} // CATE
