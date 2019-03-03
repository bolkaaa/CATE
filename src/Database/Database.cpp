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

    for (const auto path : file_paths)
    {
        add_file(path);
    }
}

void Database::add_buffer(const string &path)
{
    std::pair<string, AudioBuffer> pair(path, AudioBuffer(path));
    buffers.insert(pair);
}


void Database::load_buffers_from_db()
{
    for (auto entry : db)
    {
        const string path = entry["path"];
        add_buffer(path);
    }
}

vector<string> Database::get_filepaths() const
{
    vector<string> keys(buffers.size());

    auto key_selector = [](auto pair) { return pair.first; };

    std::transform(buffers.begin(),
                   buffers.end(),
                   keys.begin(),
                   key_selector);

    return keys;
}

vector<AudioBuffer> Database::get_buffers() const
{
    vector<AudioBuffer> values(buffers.size());

    for (auto v : buffers)
    {
        AudioBuffer b = v.second;
        b.set_chan(v.second.channels());
        b.set_sr(v.second.sample_rate());
        b.set_fname(v.second.filename());
        values.emplace_back(b);
    }

    return values;
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
    bool exists = it != buffers.end();

    return exists;
}

void Database::convert_sample_rates(double new_sr)
{
    for (auto &buffer : buffers) // Must be a reference to work correctly.
    {
        buffer.second.convert_sample_rate(new_sr);
    }
}


