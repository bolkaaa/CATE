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

#include "Granulator.hpp"

using std::vector;

namespace CATE {

Granulator::Granulator(AudioSettings *audio_settings)
        : audio_settings(audio_settings),
          grain_attack(new Param<float>(0.5f, 0.05f, 0.95f)),
          grain_sustain(new Param<float>(0.5f, 0.0f, 0.95f)),
          grain_release(new Param<float>(0.5f, 0.05f, 0.95f)),
          grain_density(new Param<float>(32.0f, 1.0f, 5000.0f)),
          grain_size(new Param<int>(4096, 1024, 8192)),
          max_grains(new FixedParam<int>({8, 12, 16, 24, 32, 48, 64}, 2)),
          scheduler(audio_settings, grain_attack, grain_sustain, grain_release, grain_density, grain_size, max_grains),
          has_buffers(false)
{
}

Granulator::~Granulator()
{
    delete max_grains;
    delete grain_size;
    delete grain_density;
    delete grain_release;
    delete grain_sustain;
    delete grain_attack;
}

float Granulator::synthesize()
{
    float output = scheduler.schedule();
    return output;
}

void Granulator::load_buffers(const AudioBufferMap &audio_buffer_map)
{
    scheduler.load_files(audio_buffer_map);
    has_buffers = true;
}

void Granulator::rebuild_grain_pool()
{
    scheduler.rebuild_grain_pool();
}

} // CATE
