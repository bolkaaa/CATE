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

#ifndef SPECTRAL_FEATURE_HPP
#define SPECTRAL_FEATURE_HPP

#include <vector>

#include "Feature.hpp"

using std::vector;

class SpectralFeature : public Feature
{
public:
    SpectralFeature(uint16_t sample_rate, uint16_t bin_size);

    /* Weighted mean of frequencies, with magnitudes as weights. Perceptually,
     * the "center of mass" of the spectrum. Characterises "brightness" of a
     * sound. */
    float centroid(const vector<float> &magspec);

    /* Ratio of geometric mean of magnitude spectrum to its arithmetic mean.
     * Characteristic of "noisiness" of a signal, where 1.0 is representative of
     * white noise, and 0.0 is a sinusoid. */
    float flatness(const vector<float> &magspec);

private:
    uint16_t bin_size;
};

#endif
