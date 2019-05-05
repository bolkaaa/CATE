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

Granulator::Granulator(AudioSettings *audio_settings, GrainParams *grain_params,
                       EnvelopeParams *env_params)
        : audio_settings(audio_settings),
          grain_params(grain_params),
          env_params(env_params),
          scheduler(audio_settings, grain_params, env_params)
{
}

float Granulator::synthesize(Unit unit)
{
    float output = scheduler.schedule(unit.marker, unit.file_path);
    return output;
}

void Granulator::load_files(const Corpus &corpus)
{
    scheduler.load_files(corpus.get_files());
}

void Granulator::rebuild_grain_pool()
{
    scheduler.rebuild_grain_pool();
}

} // CATE
