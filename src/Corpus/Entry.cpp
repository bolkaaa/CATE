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

#include "Entry.hpp"

#include <iostream>

namespace CATE {

Entry::Entry(string path)
        : path(path)
{
    markers = vector<int>();
    centroid = vector<float>();
    flatness = vector<float>();
}

Json to_json_entry(const Entry &entry)
{
    Json json_entry = {
            {"path",     entry.path},
            {"markers",  entry.markers},
            {"centroid", entry.centroid},
            {"flatness", entry.flatness}
    };

    return json_entry;
}

} // CATE
