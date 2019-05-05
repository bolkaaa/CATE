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

#include <vector>

#include <boost/filesystem.hpp>

#include "PathTree.hpp"

using std::vector;
using std::string;

namespace CATE {

PathList PathTree::get_subpaths(const Path &root_path)
{
    auto entries = vector<boost::filesystem::directory_entry>();
    PathList sub_paths;

    std::copy(boost::filesystem::directory_iterator(root_path),
              boost::filesystem::directory_iterator(),
              std::back_inserter(entries));

    for (const auto &entry : entries)
    {
        auto path = entry.path().string();
        sub_paths.push_back(path);
    }

    return sub_paths;
}

void PathTree::get_nested_files(PathList &paths, const Path &root_path)
{
    PathList sub_paths = get_subpaths(root_path);

    for (const auto &path : sub_paths)
    {
        if (boost::filesystem::is_directory(path))
        {
            get_nested_files(paths, path);
        }
        else
        {
            paths.push_back(path);
        }
    }
}

PathList PathTree::get_paths(const Path &root_path)
{
    PathList path_list;
    PathTree::get_nested_files(path_list, root_path);
    return path_list;
}

} // CATE