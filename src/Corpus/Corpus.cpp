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
        : audio_settings(audio_settings)
{
}

void Corpus::add_file(const string &path)
{
    Entry entry(path);
    Json json_object = to_json_entry(entry);
    db.push_back(json_object);
}

void Corpus::add_directory(const string &directory_path)
{
    vector<string> file_paths;
    get_nested_files(file_paths, directory_path);

    for (const auto &path : file_paths)
    {
        add_file(path);
    }
}

void Corpus::set_json_file(const string &new_file_path)
{
    db_file_path = new_file_path;
}

void Corpus::write_json_file()
{
    std::ofstream file(db_file_path);
    file << std::setw(4) << db;
}

void Corpus::read_json_data()
{
    std::ifstream ifstream(db_file_path);
    ifstream >> db;
}

void Corpus::convert_sample_rates(double new_sr)
{
    for (auto &file : files)
    {
        file.second.convert_sample_rate(new_sr);
    }
}

void Corpus::load_files()
{
    for (const auto &entry : db)
    {
        const string path = entry["path"];
        std::pair<string, AudioFile> file(path, AudioFile(path));
        files.insert(file);
    }
}

void Corpus::sliding_window_analysis()
{
    int segment_index = 0;
    FFT fft(audio_settings);

    for (const auto &file : files)
    {
        map<int, AudioBuffer> frames = segment_frames(file.second.data, audio_settings.get_buffer_size());

        for (auto &frame : frames)
        {
            int marker = frame.first;
            AudioBuffer segment = frame.second;
            fft.fill(&segment[0]);
            fft.compute_spectrum();
            fft.compute_magspec();
            vector<float> magspec = fft.get_magspec();
            Feature feature(audio_settings.get_bin_size());

            float centroid = feature.centroid(magspec);
            float flatness = feature.flatness(magspec);
            float kurtosis = feature.kurtosis(magspec);

            db[segment_index]["path"] = file.first;
            db[segment_index]["markers"].emplace_back(marker);
            db[segment_index]["centroid"].emplace_back(centroid);
            db[segment_index]["flatness"].emplace_back(flatness);
            db[segment_index]["kurtosis"].emplace_back(kurtosis);
        }

        ++segment_index;
    }
}

PointCloud Corpus::create_point_cloud()
{
    PointCloud cloud;

    for (auto entry : db)
    {
        int num_segments = entry["markers"].size();

        for (auto i = 0; i < num_segments; ++i)
        {
            int marker = entry["markers"][i];
            string file_path = entry["path"];
            float centroid = entry["centroid"][i];
            float flatness = entry["flatness"][i];
            float kurtosis = entry["kurtosis"][i];

            Point point {marker, file_path, centroid, flatness, kurtosis};

            cloud.points.emplace_back(point);
        }
    }

    return cloud;
}

bool Corpus::has_data()
{
    return !db.empty();
}


} // CATE
