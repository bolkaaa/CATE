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
#include <numeric>

#include "sndfile.hh"
#include "include/nanoflann.hpp"
#include "boost/functional/hash.hpp"

#include "Corpus.hpp"
#include "PathTree.hpp"
#include "src/Audio/AudioBuffer.hpp"
#include "src/Audio/AudioFile.hpp"
#include "KdTree.hpp"

using std::vector;
using std::unordered_map;
using std::make_pair;
using std::back_inserter;
using std::copy;
using std::pair;
using std::string;
using std::ifstream;
using std::ofstream;

namespace CATE {

Corpus::Corpus(AudioSettings *audio_settings)
        : audio_settings(audio_settings),
          point_cloud(new PointCloud()),
          out_distances(num_search_results),
          out_indices(num_search_results),
          search_results(num_search_results),
          kd_tree(Feature::num_features, *point_cloud, KDTreeSingleIndexAdaptorParams(KdTreeParams::max_leaf))
{
}

void Corpus::rebuild_index()
{
    kd_tree.buildIndex();
}

Magspec Corpus::compute_magnitude_spectrum(const AudioBuffer &buffer)
{
    FFT fft(audio_settings);

    fft.fill(buffer);
    fft.compute_spectrum();
    fft.compute_magspec();
    Magspec magspec = fft.get_magspec();

    return magspec;
}

FeatureVector Corpus::calculate_feature_vector(const AudioBuffer &buffer, int frame_size)
{
    auto feature_vector = FeatureVector();
    auto remaining_space = buffer.size() % frame_size;
    auto marker = 0;
    auto magspec = Magspec();

    for (auto it = buffer.begin(); it != (buffer.end() - remaining_space); it += frame_size)
    {
        AudioBuffer data(it, it + frame_size);
        magspec = compute_magnitude_spectrum(data);
        feature_vector.compute_all(magspec);
        feature_vector.add_marker(marker);
        marker += frame_size;
    }

    return feature_vector;
}

void Corpus::add_directory(const Path &directory_path)
{
    auto file_paths = PathTree::get_paths(directory_path);

    for (const auto &path : file_paths)
    {
        audio_buffer_map[path] = read_audio_file(path);
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

void Corpus::load_audio_from_corpus()
{
    for (const auto &entry : data.items())
    {
        const auto &path = entry.key();
        audio_buffer_map[path] = read_audio_file(path);
    }
}

void Corpus::sliding_window_analysis(int frame_size)
{
    for (auto &buffer : audio_buffer_map)
    {
        /* Calculate features for current buffer. */
        auto file_path = buffer.first;

        /* Extract markers and features vectors from audio buffer */
        auto features = calculate_feature_vector(buffer.second, frame_size);

        /* Insert markers vector into JSON object. */
        data[buffer.first]["marker"] = features.get_markers();

        /* Insert features vectors into JSON object for each dimension. */
        auto feature_map = features.get_features();

        for (const auto &feature : feature_map)
        {
            data[file_path][feature.first] = feature.second;
        }
    }
}

void Corpus::rebuild_point_cloud()
{
    point_cloud->clear();

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

            auto extractor_names = feature_vector.get_extractor_names();
            for (const auto &name : extractor_names)
            {
                auto value = segment.value()[name][i];
                point.features.emplace_back(value);
            }

            point_cloud->add(point);
        }
    }
}

void Corpus::search(const float *query)
{
    kd_tree.knnSearch(query, num_search_results, &out_indices[0], &out_distances[0]);

    for (auto i = 0; i < num_search_results; ++i)
    {
        auto point_index = out_indices[i];
        auto point = point_cloud->get_point(point_index);
        search_results[i] = point;
    }
}

int Corpus::get_num_segments()
{
    return 0;
}

AudioFrameMap Corpus::create_audio_frame_map()
{
    auto audio_frame_map = AudioFrameMap();
    AudioBuffer segment(audio_settings->get_bin_size());

    for (auto &item : data.items())
    {
        auto path = item.key();
        auto buffer = read_audio_file(path);

        /* Iterate over markers, creating AudioBuffer segments and add each segment to the AudioFrameMap hash table. */
        for (auto marker : item.value()["marker"])
        {
            auto frame = make_pair(marker, path);
            auto position = static_cast<int>(marker);

            for (auto i = 0; i < audio_settings->get_bin_size(); ++i)
            {
                segment[i] = buffer[i + position];
            }

            audio_frame_map[frame] = segment;
        }
    }

    return audio_frame_map;
}

} // CATE
