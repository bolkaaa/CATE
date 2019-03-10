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
#include "../../include/nanoflann.hpp"

#include "FileTree.hpp"
#include "../Audio/AudioBuffer.hpp"
#include "../Audio/AudioFile.hpp"
#include "../Analysis/FFT.hpp"
#include "../Analysis/SpectralFeature.hpp"
#include "Entry.hpp"
#include "PointCloud.hpp"

using std::vector;
using std::unordered_map;
using std::string;
using CATE::AudioBuffer;
using CATE::FFT;
using CATE::SpectralFeature;
using Json = nlohmann::json;

/* The Database class handles functionality for persistently storing a
 * collection of audio file paths and associated segmentation markers and
 * analysis data, forming the basis for the corpus of the concatenative
 * synthesis system. */

namespace CATE {

class Database
{
public:
    /* Add a single file to the database. */
    void add_file(const string &path);

    /* Add all files deeper than specified directory to the database. */
    void add_directory(const string &directory_path);

    /* Iterate over JSON database and load audio files into <buffers>. */
    void load_buffers_from_db();

    /* For each audio file in database, compute audio features for a sliding
     * window of <frames_per_buffer> samples,and store in a JSON file. */
    void sliding_window_analysis(int bin_size, int frames_per_buffer, const string &output_path);

    /* Convert all buffers in database to a new sample rate. */
    void convert_sample_rates(double new_sr);

    /* Save the JSON database to a JSON file, with pretty printing. */
    void to_json_file(const string &path);

    /* Read JSON file into memory. */
    void read_json_file(const string &path);

    /* From features in database, create point cloud to be used by KNN search. */
    PointCloud create_point_cloud();

    /* Get a particular file from the database, indexed by filename. */
    AudioFile &operator[](const string &file_path) { return buffers[file_path]; }

private:
    /* JSON data object, storing file paths, segmentation frame markers and
     * analysis data. */
    Json db;

    /* Hash map of audio files indexed by file names. */
    unordered_map<string, AudioFile> buffers;
};

} // CATE

#endif
