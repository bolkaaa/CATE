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

#include <nlohmann/json.hpp>
#include "../../include/nanoflann.hpp"

#include "FileTree.hpp"
#include "Entry.hpp"
#include "PointCloud.hpp"
#include "src/Audio/AudioBuffer.hpp"
#include "src/Audio/AudioFile.hpp"
#include "src/Analysis/FFT.hpp"
#include "src/Analysis/SpectralFeature.hpp"
#include "src/Synthesis/Util.hpp"

using std::vector;
using std::string;
using Json = nlohmann::json;
using CATE::AudioBuffer;
using CATE::FFT;
using CATE::SpectralFeature;

namespace CATE {

/* The Database class handles functionality for persistently storing a
 * collection of audio file paths and associated segmentation markers and
 * analysis data, forming the basis for the corpus of the concatenative
 * synthesis system. */

class Database
{
public:
    /* Add a single file to the database. */
    void add_file(const string &path);

    /* Add all files deeper than specified directory to the database. */
    void add_directory(const string &directory_path);

    /* Iterate over JSON database and load audio files into <files>. */
    void load_files();

    /* For each audio file in database, compute audio features for a sliding
     * window of <frames_per_buffer> samples,and store in a JSON file. */
    void sliding_window_analysis(int bin_size, int frames_per_buffer, const string &output_path);

    /* Convert all files in database to a new sample rate. */
    void convert_sample_rates(double new_sr);

    /* Save the JSON database to a JSON file, with pretty printing. */
    void to_json_file(const string &path);

    /* Read JSON file into memory. */
    void read_json_file(const string &path);

    /* From features in database, create point cloud to be used by KNN search. */
    PointCloud create_point_cloud();

    /* Get access to vector of audio files. */
    vector<AudioFile> get_files() { return files; };

    void segment_files(float grain_size);

    /* Get a particular file from the database, indexed by filename. */
    AudioFile &operator[](const int i) { return files[i]; }

private:
    /* JSON data object, storing file paths, segmentation frame markers and
     * analysis data. */
    Json db;

    /* The audio files stored in the database. */
    vector<AudioFile> files;
};

} // CATE

#endif
