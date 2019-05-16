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

#ifndef FEATUREVECTOR_HPP
#define FEATUREVECTOR_HPP

#include <vector>
#include <unordered_map>

#include "FFT.hpp"

using std::vector;
using std::string;
using std::unordered_map;

namespace CATE {

/* Function pointer to feature extraction function. */
typedef float (*feature_func)(const Magspec &);

/* Provides functionality for storing and manipulating collection of feature extractors. */
class FeatureVector
{
public:
    FeatureVector();

    /* Feature value of specified feature at given index. */
    inline float get(const string &feature_name, int index) { return features[feature_name][index]; }

    /* Compute all features for given magspec */
    void compute_all(const Magspec &magspec);

    vector<float> compute_once(const Magspec &magspec, const string &feature_name);

    /* Add a marker to the set of markers. */
    void add_marker(int marker);

    /* Get vector of markers (frame positions). */
    inline vector<int> get_markers() const { return markers; }

    /* Get map of feature names to feature vectors. */
    inline unordered_map <string, vector<float> > get_features() const { return features; }

    /* Get names of feature extractors. */
    vector<string> get_extractor_names();

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
