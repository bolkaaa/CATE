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
#include <iostream>

#include <boost/filesystem.hpp>

#include "PathTree.hpp"

using std::vector;
using std::string;
using std::copy;
using std::back_inserter;
using boost::filesystem::directory_iterator;
using boost::filesystem::is_directory;
using boost::filesystem::directory_entry;

namespace CATE {

PathList PathTree::get_subpaths(const Path &root_path)
{
    auto entries = vector<directory_entry>();
    auto sub_paths = PathList();

    /* Try to copy all entries within root path to entries vector. */
    try
    {
        auto begin = directory_iterator(root_path);
        auto end = directory_iterator();
        copy(begin, end, back_inserter(entries));
    } catch (const boost::filesystem::filesystem_error& e)
    {
        std::cerr << e.what();
        return PathList();
    }

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
        if (is_directory(path))
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
