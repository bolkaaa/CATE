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
#include <unordered_map>
#include <iomanip>

#include "Database.hpp"
#include "FileTree.hpp"
#include "../Audio/AudioBuffer.hpp"

using std::vector;
using std::unordered_map;
using std::pair;
using std::string;

void Database::add_file(const string &path)
{
    Entry entry(path);
    Json json_object = to_json_entry(entry);
    db.push_back(json_object);
}

void Database::add_directory(const string &directory_path)
{
    vector<string> file_paths;
    get_nested_files(file_paths, directory_path);

    for (auto path :file_paths)
    {
        add_file(path);
    }
}

void Database::load_buffers()
{
    /* For entry in Json db, buffers.push_back(AudioBuffer(entry)) */
}

vector<string> Database::get_keys() const
{
    vector<string> keys(buffers.size());

    auto key_selector = [](auto pair) { return pair.first; };

    std::transform(buffers.begin(),
                   buffers.end(),
                   keys.begin(),
                   key_selector);

    return keys;
}

std::ostream& operator<<(std::ostream& os, const Database& database)
{
    os << database.db << "\n";

    return os;
}

void Database::clear_buffers()
{
    buffers.clear();
}

void Database::to_json_file(const string &path)
{
    std::ofstream file(path);
    file << std::setw(4) << db << std::endl;
}

bool Database::buffer_exists(const string &key)
{
    auto it = buffers.find(key);

    if (it != buffers.end())
    {
        return true;
    }

    return false;
}

AudioBuffer Database::get_buffer(const string &file_path)
{
    return buffers[file_path];
}

void Database::convert_sample_rates(uint32_t new_sr)
{
    for (auto &buffer : buffers) // Must be a reference to work correctly.
    {
        buffer.second.convert_sample_rate(new_sr);
    }
}


