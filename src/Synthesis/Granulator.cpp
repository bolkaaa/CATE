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
          grain_density(new Param<float>(512.0f, 1.0f, 1024.0f)),
          grain_size(new Param<float>(50.0f, 1.0f, 200.0f)),
          grain_pitch(new Param<float>(1.0f, 0.5f, 2.0f)),
          max_grains(new FixedParam<int>({8, 12, 16, 24, 32, 48, 64}, 4)),
          scheduler(audio_settings, grain_attack, grain_sustain, grain_release, grain_density, grain_size, grain_pitch,
          max_grains)
{
}

Granulator::~Granulator()
{
    delete max_grains;
    delete grain_pitch;
    delete grain_size;
    delete grain_density;
    delete grain_release;
    delete grain_sustain;
    delete grain_attack;
}

void Granulator::calculate_grain_pool(const AudioFrameMap &audio_frame_map)
{
    auto index = 0;

    /* For each frame in the audio frame map, create a Grain from the data, and store the key (marker+path pair)
     * along with an integer in the GrainIndex map. This will be used as a lookup for the GrainPool vector. */
    for (const auto &frame : audio_frame_map)
    {
        auto frame_info = frame.first;
        auto frame_data = frame.second;
        grain_pool.emplace_back(Grain(frame_data, audio_settings));
        grain_index[frame_info] = index;
        index += 1;
    }

    scheduler.rebuild_grain_pool(grain_pool, grain_index);
}

float Granulator::synthesize(RingBuffer<CorpusIndex> *audio_index_queue)
{
    auto output = scheduler.schedule(audio_index_queue);
    return output;
}

} // CATE
