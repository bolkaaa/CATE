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

#ifndef FEATUREMAP_HPP
#define FEATUREMAP_HPP

#include <vector>
#include <map>
#include <memory>

#include "src/Audio/AudioBuffer.hpp"
#include "src/Audio/AudioSettings.hpp"
#include "Feature.hpp"
#include "FFT.hpp"

using std::map;
using std::vector;
using std::string;
using std::unique_ptr;

namespace CATE {

typedef float (*Feature)(const Magspec &); // Function pointer to feature extraction function.
typedef vector<float> FeatureVector;
typedef string FeatureName;
typedef vector<string> FeatureNameVector;
typedef map<FeatureName, FeatureVector> FeatureVectorMap ;

/* A representation of a pointer to a feature extraction function and corresponding name. */
class Extractor
{
public:
    Extractor(const string &name, const Feature &feature)
            : name(name), feature(feature)
    {
    }

    string name;
    Feature feature;
};

/* Contains a std::map that associates feature extractor names with vectors of values, along with member functions
 * for manipulating them. */
class FeatureMap
{
public:
    explicit FeatureMap(const unique_ptr<AudioSettings> &audio_settings);

    /* Given a pool of audio frames, compute feature map. */
    FeatureVectorMap compute_vectors(const AudioFramePool &audio_frame_pool);

    FeatureNameVector get_feature_names() const;

    /* Used to inform program how many features there are in the system. */
    static const int num_features = 3;

private:
    /* Given a frame of audio, calculate its magnitude spectrum. */
    Magspec calculate_frame_spectrum(const AudioFrame &audio_frame);

    vector<Extractor> feature_extractors;
    FeatureVectorMap feature_vector_map;
    AudioSettings *audio_settings;
    FFT fft;

    map<string, Feature> function_map = {
            {"centroid", spectral_centroid},
            {"rolloff",  spectral_rolloff},
            {"flatness", spectral_flatness}
    };
};

} // CATE

#endif
