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

#ifndef KDTREE_HPP
#define KDTREE_HPP

#include "include/nanoflann.hpp"

#include "src/Corpus/PointCloud.hpp"
#include "src/Analysis/FeatureMap.hpp"

using CATE::PointCloud;
using nanoflann::KDTreeSingleIndexAdaptor;
using nanoflann::KDTreeSingleIndexAdaptorParams;
using nanoflann::L2_Simple_Adaptor;

namespace CATE {

/* KdTree is a typedef for functionality from the nanoflann library, with KdTreeParams storing some necessary
 * constants. */
class KdTreeParams
{
public:
    static const int max_leaf = 8;
};

typedef KDTreeSingleIndexAdaptor<L2_Simple_Adaptor<float, PointCloud>,
        PointCloud,
        FeatureMap::num_features> KdTree;

} // CATE

#endif
