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

#ifndef FILE_TREE_HPP
#define FILE_TREE_HPP

#include <vector>
#include <string>

using std::vector;
using std::string;

/* These are some simple functions that together provide functionality for
 * getting a list of all paths deeper than a specified root path. The filesystem
 * part of the Boost library is used to achieve this. */

/* Get vector of filenames containing each subpath within <root_path>. */
vector<string> get_subpaths(const string &root_path);

/* Recursively get all nested files from a root directory. Uses the
 * <get_subpaths> function to achieve this. */
void get_nested_files(vector<string> &paths, const string &root_path);

#endif
