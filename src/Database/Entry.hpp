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

#ifndef ENTRY_HPP
#define ENTRY_HPP

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

using std::string;
using std::vector;
using Json = nlohmann::json;

/* Data representation of a single-entry in the JSON database. Includes file
 * path and vectors of segmentation markers and audio features. */

namespace CATE {

class Entry
{
public:
    explicit Entry(string path);

    string path;
    vector<float> markers;
    vector<float> centroid;
    vector<float> flatness;
};

/* Helper function to convert an Entry object into a Json object. */
Json to_json_entry(const Entry &entry);

} // CATE

#endif
