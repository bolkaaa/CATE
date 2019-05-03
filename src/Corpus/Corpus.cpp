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
#include "../../include/nanoflann.hpp"

#include "Corpus.hpp"
#include "Entry.hpp"
#include "FileTree.hpp"
#include "src/Audio/AudioBuffer.hpp"
#include "src/Audio/AudioFile.hpp"

using std::vector;
using std::unordered_map;
using std::pair;
using std::string;

namespace CATE {

Corpus::Corpus(const AudioSettings &audio_settings)
        : audio_settings(audio_settings),
          feature_map(audio_settings)
{
}

void Corpus::add_directory(const string &directory_path)
{
    vector<string> file_paths;
    get_nested_files(file_paths, directory_path);

    for (const auto &path : file_paths)
    {
        files[path] = AudioFile(path);
    }
}

void Corpus::write_file(const string &file_path)
{
    std::ofstream file(file_path);
    file << data;
}

void Corpus::read_file(const string &file_path)
{
    std::ifstream ifstream(file_path);
    ifstream >> data;
}

void Corpus::convert_sample_rates(double new_sr)
{
    for (auto &file : files)
    {
        file.second.convert_sample_rate(new_sr);
    }
}

void Corpus::load_audio_from_db()
{
    for (auto entry : data)
    {
        string path = entry["path"];
        files[path] = AudioFile(path);
    }
}

void Corpus::sliding_window_analysis()
{
    for (auto &file : files)
    {
        map<int, AudioBuffer> frames = segment_frames(file.second.data, audio_settings.get_buffer_size());
        feature_map.compute_vectors(frames);
        auto features = feature_map.get_features();
        string file_path = file.first;
        vector<int> marker = get_keys<int, AudioBuffer>(frames);

        data[file.first]["marker"] = marker;
        for (auto f : features)
        {
            data[file_path][f.first] = f.second;
        }
    }
}

PointCloud Corpus::create_point_cloud()
{
    PointCloud cloud;

    for (auto &segment : data.items())
    {
        auto n = segment.value()["marker"].size();

        for (auto i = 0; i < n; ++i)
        {
            auto path = segment.key();
            auto marker = segment.value()["marker"][i];
            auto centroid = segment.value()["centroid"][i];
            auto flatness = segment.value()["flatness"][i];

            Point point{path, marker, centroid, flatness};

            cloud.add(point);
        }
    }

    return cloud;
}

bool Corpus::has_data()
{
    return !data.empty();
}

} // CATE
