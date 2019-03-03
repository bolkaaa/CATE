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

#include <nlohmann/json.hpp>

#include "FileTree.hpp"
#include "../Audio/AudioBuffer.hpp"
#include "Entry.hpp"

using std::vector;
using std::unordered_map;
using std::string;
using Json = nlohmann::json;

/*
  The Database class handles functionality for working with a collection for
  audio buffers. It will form the basis of the corpus used in the program.
 */

class Database
{
public:
    /* Add a single file to the database. */
    void add_file(const string &path);

    /* Add all files deeper than specified directory to the database.
       TODO: Add some way of ignoring non-audio files. */
    void add_directory(const string &directory_path);

    /* Iterate over JSON database and load audio files into <buffers>. */
    void load_buffers();

    /* Clear all buffers in database. */
    void clear_buffers();

    /* Convert all buffers in database to new sample rate. */
    void convert_sample_rates(uint32_t new_sr);

    /* Get the size of the database. */
    uint16_t size() const { return db.size(); }

    /* Get keys from buffers map as a vector of strings. */
    vector<string> get_keys() const;

    /* Check if a particular file exists in the database. */
    bool buffer_exists(const string &key);

    /* Return a particular buffer from the database as a single object. */
    AudioBuffer get_buffer(const string &file_path);

    /* Save the JSON database to a JSON file. */
    void to_json_file(const string &path);

    /* Get a particular buffer from the database, indexed by filename. */
    AudioBuffer& operator[](const string &file_path) { return buffers[file_path]; }

    /* Output the database as a stream. */
    friend std::ostream& operator<<(std::ostream& os, const Database& database);


private:
    /* JSON data object, storing file paths, segmentation frame markers and
     * analysis data. */
    Json db;

    /* Hash map of buffers indexed by buffer/file names. */
    unordered_map<string, AudioBuffer> buffers;

};

#endif
