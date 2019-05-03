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

#include <vector>

#include "src/Audio/AudioSettings.hpp"
#include "src/Audio/AudioBuffer.hpp"
#include "Feature.hpp"
#include "FFT.hpp"
#include "Math.hpp"

using std::vector;

namespace CATE {

/* The quotient of the sum and weighted sum of a mangnitude spectrum. Characterises "brightness" of a sound. */
float spectral_centroid(const MagSpec &magspec);

/* Ratio of geometric mean of magnitude spectrum to its arithmetic mean. Characteristic of "noisiness" of a signal. */
float spectral_flatness(const MagSpec &magspec);

} // CATE

#endif
