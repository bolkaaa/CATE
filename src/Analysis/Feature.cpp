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
#include <numeric>

#include "Feature.hpp"

namespace CATE {

Feature::Feature(int bin_size)
        : bin_size(bin_size)
{
}

float Feature::centroid(const vector<float> &magspec)
{
    auto magnitudes = 0.0f;
    auto weighted_magnitudes = 0.0f;
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
        auto c = static_cast<float>(weighted_magnitudes / magnitudes);
        return c;
    }

    return 0.0f;
}

float Feature::flatness(const vector<float> &magspec)
{
    auto sum = 0.0f;
    auto log_sum = 0.0f;
    auto n = static_cast<float>(magspec.size());
    auto flatness = 0.0f;

    for (auto &x : magspec)
    {
        sum += (1 + x);
        log_sum += std::log(1 + x);
    }

    sum /= n;
    log_sum /= n;

    /* Protect against divide-by-zero errors. */
    if (sum > 0)
    {
        flatness = std::exp(log_sum / sum);
        return flatness;
    }

    return flatness;
}

float Feature::kurtosis(const vector<float> &magspec)
{
    float m2 = 0.0f;
    float m4 = 0.0f;
    float mean = calculate_magspec_mean(magspec);

    for (auto x : magspec)
    {
        float diff = x - mean;
        m2 += (diff * diff);
        m4 += (diff * diff * diff);
    }

    m2 /= magspec.size();
    m4 /= magspec.size();

    return (m2 == 0) ? -3.0f : ((m4 / (m2 * m2)) - 3.0f);
}

float Feature::calculate_magspec_mean(const vector<float> &magspec)
{
    auto sum = std::accumulate(magspec.begin(), magspec.end(), 0.0f);
    auto mean = sum / static_cast<float>(magspec.size());
    return mean;
}

} // CATE
