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

FeatureMap::FeatureMap(AudioSettings *audio_settings)
        : audio_settings(audio_settings),
          fft(audio_settings)
{
}

FeatureVectorMap FeatureMap::compute_vectors(const AudioFramePool &audio_frames)
{
    for (auto &frame : audio_frames)
    {
        auto magspec = calculate_frame_spectrum(frame);

        for (const auto &extractor : feature_extractors)
        {
            auto value = extractor.feature(magspec);
            feature_vector_map[extractor.name].emplace_back(value);
        }
    }

    return feature_vector_map;
}

Magspec FeatureMap::calculate_frame_spectrum(const AudioFrame &audio_frame)
{
    auto buffer = audio_frame.second;

    fft.fill(&buffer[0], buffer.size());
    fft.compute_spectrum();
    fft.compute_magspec();
    auto magspec = fft.get_magspec();

    return magspec;
}

FeatureNameVector FeatureMap::get_feature_names() const
{
    FeatureNameVector feature_names;

    for (const auto &feature : function_map)
    {
        feature_names.emplace_back(feature.first);
    }

    return feature_names;
}

} // CATE

