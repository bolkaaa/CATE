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

#ifndef POINTCLOUD_HPP
#define POINTCLOUD_HPP

#include <vector>
#include <utility>
#include <cstdlib>
#include <iostream>

#include "src/Analysis/FeatureMap.hpp"
#include "src/Util/NonCopyable.hpp"

using std::vector;
using std::string;

namespace CATE {

/* Data structure of a single unit point used in analysis process. */
class Point
{
public:
    string file_path;
    int marker;
    vector<float> features;
};

/* Stores a container of data points and functions used by K-d tree system. */
class PointCloud : NonCopyable
{
public:
    /* Add a point to the cloud. */
    inline void add(const Point &p) { points.emplace_back(p); }

    /* Clear all points from cloud. */
    void clear() { points.clear(); };

    /* Return number of data points. */
    inline int kdtree_get_point_count() const { return points.size(); }

    /* Get element at dimension dim of point i in class. */
    inline float kdtree_get_pt(const int i, const int dim) const
    {
        return points[i].features[dim];
    }

    inline Point get_point(const int i) { return points[i]; }

    template<class BBOX>
    bool kdtree_get_bbox(BBOX &) const { return false; }

private:
    vector<Point> points;
};

} // CATE

#endif
