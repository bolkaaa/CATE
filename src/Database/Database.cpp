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

#include "sndfile.hh"

#include "Database.hpp"
#include "Entry.hpp"
#include "FileTree.hpp"
#include "../Audio/AudioBuffer.hpp"
#include "../Audio/AudioFile.hpp"

using std::vector;
using std::unordered_map;
using std::pair;
using std::string;
using CATE::Entry;
using CATE::AudioFile;

namespace CATE {

void Database::add_file(const string &path)
{
    Entry entry(path);
    Json json_object = to_json_entry(entry);
    db.push_back(json_object);
}

void Database::add_directory(const string &directory_path)
{
    vector<string> file_paths;
    CATE::get_nested_files(file_paths, directory_path);

    for (const auto path : file_paths)
    {
        add_file(path);
    }
}

vector<string> Database::get_paths() const
{
    vector<string> paths(buffers.size());

    auto selector = [](auto pair) { return pair.first; };

    std::transform(buffers.begin(),
                   buffers.end(),
                   paths.begin(),
                   selector);

    return paths;
}

vector<AudioFile> Database::get_files() const
{
    vector<AudioFile> files(buffers.size());

    auto selector = [](auto pair) { return pair.second; };

    std::transform(buffers.begin(),
                   buffers.end(),
                   files.begin(),
                   selector);

    return files;
}

std::ostream &operator<<(std::ostream &os, const Database &database)
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
    for (auto &buffer : buffers)
    {
        buffer.second.convert_sample_rate(new_sr);
    }
}

void Database::load_buffers_from_db()
{
    for (const auto entry : db)
    {
        const string path = entry["path"];
        AudioFile file(path);
        std::pair<string, AudioFile> pair(path, file);
        buffers.insert(pair);
    }
}

} // CATE
