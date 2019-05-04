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

FeatureMap::FeatureMap(const unique_ptr<AudioSettings> &audio_settings)
        : audio_settings(audio_settings.get()),
          fft(audio_settings.get())
{
    populate_extractors();
}

void FeatureMap::populate_extractors()
{
    map<string, Feature> function_map = {
            {"centroid", spectral_centroid},
            {"rolloff", spectral_rolloff},
            {"flatness", spectral_flatness}
    };

    for (const auto &elem : function_map)
    {
        string name = elem.first;
        Feature feature = elem.second;
        Extractor extractor{name, feature};
        feature_extractors.emplace_back(extractor);
        feature_map[name] = vector<float>();
    }
}

void FeatureMap::compute_vectors(map<int, AudioBuffer> audio_frames)
{
    for (auto &frame : audio_frames)
    {
        Magspec magspec = calculate_frame_spectrum(frame);

        for (auto &extractor : feature_extractors)
        {
            float value = extractor.feature(magspec);
            feature_map[extractor.name].emplace_back(value);
        }
    }
}

vector<float> FeatureMap::calculate_frame_spectrum(const pair<int, AudioBuffer> &frame)
{
    AudioBuffer buffer(frame.second);

    fft.fill(&buffer[0], buffer.size());
    fft.compute_spectrum();
    fft.compute_magspec();
    Magspec magspec = fft.get_magspec();

    return magspec;
}

} // CATE

