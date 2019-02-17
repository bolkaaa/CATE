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

#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <vector>
#include <unordered_map>

#include "AudioBuffer.hpp"
#include "FileTree.hpp"

template <class T>
class Database
{
public:
    /* Default constructor. */
    Database();

    /* Add a single file to the database. */
    void add_file(const std::string &path);

    /* Add all files deeper than specified directory to the database.
       TODO: Add some way of ignoring non-audio files. */
    void add_directory(const std::string &directory_path);

    /* Clear all buffers in database. */
    void clear();

    /* Convert all buffers in database to new sample rate. */
    void convert_sample_rates(uint32_t new_sr);

    /* Get a particular buffer from the database, indexed by filename. */
    AudioBuffer<T>& operator[](std::string filename) { return buffers[filename]; }

    /* Get the size of the database. */
    uint16_t size() const { return buffers.size(); }

    /* Check if a particular file exists in the database. */
    bool exists(std::string key);

    /* Return a particular buffer from the database as a single object. */
    AudioBuffer<T> get_buffer(std::string buffer_name);

private:
    /* Hash table of buffers indexed by filenames. */
    std::unordered_map<std::string, AudioBuffer<T>> buffers;

};

template <class T>
Database<T>::Database()
{
}

template <class T>
void Database<T>::add_file(const std::string &path)
{
    AudioBuffer<T> buffer(path);
    std::pair<std::string, AudioBuffer<T>> pair(path, buffer);
    buffers.insert(pair);
}

template <class T>
void Database<T>::add_directory(const std::string &directory_path)
{
    std::vector<std::string> file_paths;
    get_nested_files(file_paths, directory_path);

    for (auto path :file_paths)
    {
        AudioBuffer<T> buffer(path);
        std::pair<std::string, AudioBuffer<T>> pair(path, buffer);
        buffers.insert(pair);
    }
}

template <class T>
void Database<T>::clear()
{
    buffers.clear();
}

template <class T>
void Database<T>::convert_sample_rates(uint32_t new_sr)
{
    /* Note: must be a reference to work correctly. */
    for (auto &buffer : buffers)
    {
        buffer.second.convert_sample_rate(new_sr);
    }
}

template <class T>
bool Database<T>::exists(std::string key)
{
    auto it = buffers.find(key);

    if (it != buffers.end())
    {
        return true;
    }

    return false;
}

template <class T>
AudioBuffer<T> Database<T>::get_buffer(std::string buffer_name)
{
    return buffers[buffer_name];
}

#endif
