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
#include "include/nanoflann.hpp"

#include "Corpus.hpp"
#include "PathTree.hpp"
#include "src/Audio/AudioBuffer.hpp"
#include "src/Audio/AudioFile.hpp"
#include "KdTree.hpp"

using std::vector;
using std::unordered_map;
using std::pair;
using std::string;
using std::ifstream;
using std::ofstream;

namespace CATE {

Corpus::Corpus(AudioSettings *audio_settings)
        : audio_settings(audio_settings),
          kd_tree(new KdTree(num_features, point_cloud, KDTreeSingleIndexAdaptorParams(KdTreeParams::max_leaf))),
          feature_map(audio_settings),
          feature_names(feature_map.get_feature_names())
{
}

void Corpus::rebuild_index()
{
    kd_tree->buildIndex();
}

void Corpus::add_directory(const Path &directory_path)
{
    auto file_paths = PathTree::get_paths(directory_path);

    for (const auto &path : file_paths)
    {
        files[path] = AudioFile(path);
    }
}

void Corpus::write_file(const Path &file_path)
{
    auto file = ofstream(file_path);
    file << std::setw(4) << data;
}

void Corpus::read_file(const Path &file_path)
{
    auto file = ifstream(file_path);
    file >> data;
}

void Corpus::load_audio_from_db()
{
    for (const auto &entry : data.items())
    {
        const auto &path = entry.key();
        files[path] = AudioFile(path);
    }
}

void Corpus::sliding_window_analysis()
{
    auto frame_size = audio_settings->get_bin_size().value;
    const auto hop_size = frame_size / 2;

    for (auto &file : files)
    {
        AudioFramePool audio_frame_pool = segment_frames(file.second.data, frame_size, hop_size);

        auto features = feature_map.compute_vectors(audio_frame_pool);
        auto markers = get_keys<int, AudioBuffer>(audio_frame_pool);
        auto file_path = file.first;

        /* Insert markers vector into JSON object. */
        data[file.first]["marker"] = markers;

        /* Insert features vectors into JSON object for each dimension. */
        for (const auto &f : features)
        {
            data[file_path][f.first] = f.second;
        }
    }
}

void Corpus::rebuild_point_cloud()
{
    point_cloud.clear();

    for (auto &segment : data.items())
    {
        auto n = segment.value()["marker"].size();

        for (auto i = 0; i < n; ++i)
        {
            auto point = Point();

            /* Add file path and marker to point. */
            point.file_path = segment.key();
            point.marker = segment.value()["marker"][i];

            /* Add each feature dimension to point. */
            for (const auto &f : feature_names)
            {
                auto value = segment.value()[f][i];
                point.features.emplace_back(value);
            }

            point_cloud.add(point);
        }
    }
}

bool Corpus::has_data()
{
    return !data.empty();
}

} // CATE
