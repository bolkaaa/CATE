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
#include <cstdlib>
#include <iostream>

using std::vector;
using std::string;

namespace CATE {

/* Data structure of points used in analysis process. */
class Point
{
public:
    int marker;
    string file_path;
    float centroid;
    float flatness;
    float kurtosis;
};

/* Stores a container of data points and functions used by K-d tree system. */
class PointCloud
{
public:
    vector<Point> points;

    /* Return number of data points. */
    inline int kdtree_get_point_count() const
    { return points.size(); }

    /* Get element at dimension dim of point i in class. */
    inline float kdtree_get_pt(const int i, const int dim) const
    {
        if (dim == 0)
        {
            return points[i].centroid;
        } else
        {
            return points[i].flatness;
        }
    }

    template<class BBOX>
    bool kdtree_get_bbox(BBOX &) const
    { return false; }
};

} // CATE

#endif
