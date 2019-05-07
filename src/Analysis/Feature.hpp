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

#ifndef FEATURE_HPP
#define FEATURE_HPP

#include <unordered_map>

#include "FFT.hpp"
#include "src/Util/Util.hpp"

using std::unordered_map;

namespace CATE {

/* Typedef to feature extraction function. */
typedef float (*feature_func)(const Magspec &);

namespace Feature {

/* The quotient of the sum and weighted sum of a magnitude spectrum. Characterises "brightness" of a sound. */
float spectral_centroid(const Magspec &magspec);

/* Frequency ratio beneath which a particular percentage (set to 85%) of the spectrum's energy is contained. Ex: an
 * output of 0.5 would mean that 85% of the spectrum's energy is contained within the first half of the spectrum. */
float spectral_rolloff(const Magspec &magspec);

/* Ratio of geometric mean of magnitude spectrum to its arithmetic mean. Characteristic of "noisiness" of a signal. */
float spectral_flatness(const Magspec &magspec);

const static int num_features = 3;

} // Feature

/* Store markers as vectors and feature data as unordered map keyed by feature name. */
class FeatureVector
{
public:
    FeatureVector()
    {
        /* Populate extractors with feature name and pointer to corresponding function. */
        extractors.emplace_back(FeatureExtractor{"centroid", Feature::spectral_centroid});
        extractors.emplace_back(FeatureExtractor{"flatness", Feature::spectral_flatness});
        extractors.emplace_back(FeatureExtractor{"rolloff", Feature::spectral_rolloff});
    }

    /* Compute all features for given magspec */
    void compute_all(const Magspec &magspec)
    {
        for (auto i = 0; i < Feature::num_features; ++i)
        {
            auto name = extractors[i].name;
            auto value = extractors[i].function(magspec);
            features[name].emplace_back(value);
        }
    }

    /* Add a marker to the set of markers. */
    void add_marker(int marker)
    {
        markers.emplace_back(marker);
    }

    /* Get vector of markers (frame positions). */
    inline vector<int> get_markers() const { return markers; }

    /* Get map of feature names to feature vectors. */
    unordered_map<string, vector<float> > get_features() { return features; }

    /* Get names of feature extractors. */
    vector<string> get_extractor_names()
    {
        vector<string> names;

        for (const auto &e : extractors)
        {
            names.emplace_back(e.name);
        }

        return names;
    }

private:
    /* Structure to store feature extraction function and respective name. */
    class FeatureExtractor
    {
    public:
        string name;
        feature_func function;
    };

    vector<int> markers;
    unordered_map<string, vector<float> > features;
    vector<FeatureExtractor> extractors;
};

} // CATE

#endif
