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

#include "src/Audio/AudioBuffer.hpp"
#include "src/Audio/AudioSettings.hpp"
#include "Feature.hpp"
#include "FFT.hpp"

using std::map;
using std::vector;
using std::string;
using std::pair;

namespace CATE {

/* Typedef for function pointer to feature extraction function. */
typedef float (*Feature)(const Magspec&);

/* Typedef for container of feature values. */
typedef vector<float> FeatureVector;

/* A representation of a pointer to a feature extraction function and corresponding name. */
class Extractor
{
public:
    string name;
    Feature feature;
};

/* Contains a std::map that associates feature extractor names with vectors of values, along with member functions
 * for manipulating them. */
class FeatureMap
{
public:
    explicit FeatureMap(const AudioSettings &audio_settings);

    /* Given a pool of audio frames, compute feature map. */
    void compute_vectors(AudioFramePool audio_frames);

    /* Get feature map. */
    inline map<string, FeatureVector> get_features() const { return feature_map; }

    /* Used to inform the rest of the system of how many features are present. */
    static const int num_features = 2;

private:
    /* Given a frame of audio, calculate its magnitude spectrum. */
    Magspec calculate_frame_spectrum(const AudioFrame &frame);

    /* Fill vector of feature extractor function pointers. */
    void populate_extractors();

    map<string, FeatureVector> feature_map;
    vector<Extractor> feature_extractors;
    const AudioSettings &audio_settings;
    FFT fft;
};

} // CATE

#endif
