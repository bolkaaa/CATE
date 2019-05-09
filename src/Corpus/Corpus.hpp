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
#include "src/Analysis/Feature.hpp"
#include "src/Util/NonCopyable.hpp"

using std::unordered_map;
using std::vector;
using std::string;
using std::pair;
using std::unique_ptr;
using Json = nlohmann::json;

namespace CATE {


/* Handles functionality for persistently storing a
 * collection of audio file paths and associated segmentation markers and
 * analysis data, forming the basis for the corpus of the concatenative
 * synthesis system. */
class Corpus : NonCopyable
{
public:
    Corpus(AudioSettings *audio_settings);

    /* For each audio file in the corpus, compute audio features for a sliding
     * window of <frames_per_buffer> samples, and store in file. */
    void sliding_window_analysis(int frame_size);

    /* Create hash table data structure where keys are pairs of audio file path and file positions. */
    AudioFrameMap create_audio_frame_map();

    /* Get number of audio file segments from corpus. */
    int get_num_segments();

    /* Search for nearest neighbours. */
    void search(const float *query);

    /* Rebuild k-d tree index. */
    void rebuild_index();

    /* Get marker/path pair from search results. */
    inline pair<int, string> get_search_result(const int i) const { return make_pair(search_results[i].marker,
            search_results[i].file_path); }

    /* Read analysis data file into memory. */
    void read_file(const Path &file_path);

    /* Save the analysis data to file, with pretty printing. */
    void write_file(const Path &file_path);

    /* Add all files deeper than specified directory to the database. */
    void add_directory(const Path &directory_path);

    /* Iterate over paths in db object and load audio files into map. */
    void load_audio_from_corpus();

    /* From features in database, create point cloud to be used by KNN search. */
    void rebuild_point_cloud();

    inline int size() const { return point_cloud->kdtree_get_point_count(); }

    inline int get_num_files() const { return audio_buffer_map.size(); }

private:
    /* Compute magnitude spectrum for given audio buffer. */
    Magspec compute_magnitude_spectrum(const AudioBuffer &buffer);

    /* Given an audio buffer and frame size, calculate markers and features. */
    FeatureVector calculate_feature_vector(const AudioBuffer &buffer, int frame_size);

    Json data;
    AudioSettings *audio_settings;
    PointCloud *point_cloud;
    const size_t num_search_results = 32;
    vector<size_t> out_indices;
    vector<Point> search_results;
    vector<float> out_distances;
    KdTree kd_tree;
    AudioBufferMap audio_buffer_map;
    FeatureVector feature_vector;
};


} // CATE

#endif
