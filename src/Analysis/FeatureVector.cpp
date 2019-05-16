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

#include "Feature.hpp"
#include "FeatureVector.hpp"

namespace CATE {

FeatureVector::FeatureVector()
{
    /* Populate extractors with feature name and pointer to corresponding function. */
    extractors.emplace_back(FeatureExtractor{"centroid", Feature::spectral_centroid});
    extractors.emplace_back(FeatureExtractor{"flatness", Feature::spectral_flatness});
    extractors.emplace_back(FeatureExtractor{"rolloff", Feature::spectral_rolloff});
}

void FeatureVector::compute_all(const Magspec &magspec)
{
    for (const auto &extractor : extractors)
    {
        auto name = extractor.name;
        auto value = extractor.function(magspec);
        features[name].emplace_back(value);
    }
}

void FeatureVector::add_marker(int marker)
{
    markers.emplace_back(marker);
}

vector<string> FeatureVector::get_extractor_names()
{
    vector<string> names;

    for (const auto &extractor : extractors)
    {
        names.emplace_back(extractor.name);
    }

    return names;
}

} // CATE
