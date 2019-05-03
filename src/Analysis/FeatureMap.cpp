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

#include "FeatureMap.hpp"

namespace CATE {

FeatureMap::FeatureMap(const AudioSettings &audio_settings)
        : audio_settings(audio_settings),
          fft(audio_settings)
{
    populate_extractors();
}

void FeatureMap::populate_extractors()
{
    feature_extractors.emplace_back(Extractor{spectral_centroid, "centroid"});
    feature_extractors.emplace_back(Extractor{spectral_flatness, "flatness"});
}

void FeatureMap::compute_vectors(map<int, AudioBuffer> audio_frames)
{
    for (auto &extractor : feature_extractors)
    {
        for (auto &frame : audio_frames)
        {
            vector<float> magspec = calculate_frame_spectrum(frame);
            float value = extractor.function(magspec);
            feature_map[extractor.name].emplace_back(value);
        }
    }
}

vector<float> FeatureMap::calculate_frame_spectrum(const pair<int, AudioBuffer> &frame)
{
    AudioBuffer buffer(frame.second);

    fft.fill(&buffer[0]);
    fft.compute_spectrum();
    fft.compute_magspec();
    vector<float> magspec = fft.get_magspec();

    return magspec;
}

} // CATE

