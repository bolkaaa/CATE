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
#include "Math.hpp"

namespace CATE {
namespace Feature {

float spectral_centroid(const Magspec &magspec)
{
    auto sum = 0.0f;
    auto weighted_sum = 0.0f;
    auto i = 1;

    for (auto k : magspec)
    {
        sum += k;
        weighted_sum += k * i;
        i += 1;
    }

    if (sum == 0.0f)
    {
        return sum;
    }

    return weighted_sum / sum;
}

float spectral_flatness(const Magspec &magspec)
{
    auto a = Math::geometric_mean<float, float>(magspec);
    auto b = Math::arithmetic_mean<float, float>(magspec);

    if (b == 0)
    {
        return 0.0f;
    }

    return a / b;
}

float spectral_rolloff(const Magspec &magspec)
{
    const float cutoff = 0.85f;
    auto total_energy = std::accumulate(magspec.begin(), magspec.end(), 0.0);
    const auto energy_at_cutoff = cutoff * total_energy;
    auto energy = 0.0f;
    auto i = 0;
    auto bin_index = 0;

    /* Find bin index at which accumulated energy at that point exceeds the cutoff energy. */
    for (auto x : magspec)
    {
        energy += x;

        if (energy > energy_at_cutoff)
        {
            bin_index = i;
            break;
        }

        ++i;
    }

    auto rolloff = static_cast<float>(bin_index) / static_cast<float>(magspec.size());

    return rolloff;
}

} // Feature

} // CATE
