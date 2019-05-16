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

#ifndef MATH_HPP
#define MATH_HPP

#include <vector>
#include <cmath>

using std::vector;
using std::log;
using std::exp;

namespace CATE {

namespace Math {

/* Return geometric mean (Nth root of product of N values) of vector. */
template<class A, class B>
inline A geometric_mean(const vector<B> &v)
{
    auto log_sum = 0.0f;
    auto min = 0.01; // term added to x to avoid log(0) operations

    for (auto x : v)
    {
        log_sum += log(x + min);
    }

    auto mean = exp(log_sum / v.size());

    return mean;
}

/* Return arithmetic mean (sum of N values divided by n) of vector. */
template<class A, class B>
inline A arithmetic_mean(const vector<B> &v)
{
    auto sum = 0.0f;

    for (auto x : v)
    {
        sum += x;
    }

    auto mean = sum / v.size();

    return mean;
}

} // Math

} // Cate

#endif //MATH_HPP
