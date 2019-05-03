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

class Math
{
public:
    /* Return geometric mean (Nth root of product of N values) of vector. */
    template <class A, class B>
    inline static A geometric_mean(const vector<B> &v)
    {
        auto log_sum = 0.0f;

        for (auto x : v)
        {
            log_sum += log(x);
        }

        return exp(log_sum / v.size());
    }

    /* Return arithmetic mean (sum of N values divided by n) of vector. */
    template <class A, class B>
    inline static A arithmetic_mean(const vector<B> &v)
    {
        auto sum = 0.0f;

        for (auto x : v)
        {
            sum += x;
        }

        return sum / v.size();
    }

};


#endif //MATH_HPP
