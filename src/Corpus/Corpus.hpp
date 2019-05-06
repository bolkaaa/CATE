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

#include <memory>
#include <vector>

#include <nlohmann/json.hpp>
#include "include/nanoflann.hpp"

#include "KdTree.hpp"
#include "PointCloud.hpp"
#include "PathTree.hpp"
#include "src/Audio/AudioBuffer.hpp"
#include "src/Audio/AudioFile.hpp"
#include "src/Analysis/FeatureMap.hpp"
#include "src/Util/NonCopyable.hpp"

using std::vector;
using std::string;
using std::pair;
using std::unique_ptr;
using Json = nlohmann::json;

namespace CATE {

/* Representation of single position in file in corpus. */
class Unit
{
public:
    string file_path;
    int marker;
};

/* Handles functionality for persistently storing a
 * collection of audio file paths and associated segmentation markers and
 * analysis data, forming the basis for the corpus of the concatenative
 * synthesis system. */
class Corpus : NonCopyable
{
public:
    Corpus(AudioSettings *audio_settings);

    /* Rebuild k-d tree index. */
    void rebuild_index();

    /* Search for nearest neighbours. */
    void search(const float *query);

    /* Get unit from search results. */
    inline Point get_search_result(const int i) const { return search_results[i]; };

    /* Read analysis data file into memory. */
    void read_file(const string &file_path);

    /* Save the analysis data to file, with pretty printing. */
    void write_file(const string &file_path);

    /* Add all files deeper than specified directory to the database. */
    void add_directory(const string &directory_path);

    /* Iterate over paths in db object and load audio files into map. */
    void load_audio_from_db();

    /* For each audio file in database, compute audio features for a sliding
     * window of <frames_per_buffer> samples, and store in file. */
    void sliding_window_analysis();

    /* From features in database, create point cloud to be used by KNN search. */
    void rebuild_point_cloud();

    inline int size() const { return point_cloud->kdtree_get_point_count(); }

    inline int get_num_files() const { return files.size(); }

    /* Get access to audio files. */
    inline map<string, AudioFile> get_files() const { return files; };

private:
    /* Given a std::map, get a std::vector of its keys. */
    template <class A, class B>
    static inline vector<A> get_keys(map<A, B> m)
    {
        vector<A> keys;

        for (auto &elem : m)
        {
            keys.emplace_back(elem.first);
        }

        return keys;
    }

    Json data;
    FeatureMap feature_map;
    AudioSettings *audio_settings;
    FeatureNameVector feature_names;
    PointCloud *point_cloud;
    const int num_features = 3;
    const size_t num_search_results = 32;
    vector<size_t> out_indices;
    vector<Point> search_results;
    vector<float> out_distances;
    KdTree kd_tree;
    map<string, AudioFile> files;
};


} // CATE

#endif
