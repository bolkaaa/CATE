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

#include <cmath>

#include "Feature.hpp"
#include "SpectralFeature.hpp"

SpectralFeature::SpectralFeature(uint16_t sample_rate, uint16_t bin_size)
    : Feature(sample_rate),
      bin_size(bin_size)
{
}

float SpectralFeature::centroid(const vector<float> &magspec)
{
    auto magnitudes = 0.0;
    auto weighted_magnitudes = 0.0;
    auto i = 0;

    for (auto x : magspec)
    {
        magnitudes += x;
        weighted_magnitudes += (x * i);
        ++i;
    }

    /* Protect against divide-by-zero errors. */
    if (magnitudes > 0)
    {
        float c = weighted_magnitudes / magnitudes;
        return c;
    }

    return 0.0;
}

float SpectralFeature::flatness(const vector<float> &magspec)
{
    double sum = 0.0;
    double log_sum = 0.0;
    double n = (double) (magspec.size());

    for (auto x : magspec)
    {
        double v = static_cast<double>(1 + x);
        sum += v;
        log_sum += std::log(v);
    }

    sum /= n;
    log_sum /= n;

    /* Protect against divide-by-zero errors. */
    if (sum > 0)
    {
        float f = static_cast<float>(std::exp(log_sum / sum));
        return f;
    }

    return 0.0;
}
