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

#ifndef RAND_HPP
#define RAND_HPP

#include <random>

using std::random_device;
using std::mt19937;
using std::uniform_real_distribution;

namespace CATE {

template<class T>
class Rand
{
public:
    Rand(T min, T max);

    /* Get a random value from the distribution. */
    float get();

private:
    mt19937 gen;
    uniform_real_distribution<T> dist;
};

template<class T>
Rand<T>::Rand(T min, T max)
    : gen(random_device{}())
{
}

template<class T>
float Rand<T>::get()
{
    return dist(gen);
}

} // CATE

#endif
