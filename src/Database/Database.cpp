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

#include "Database.hpp"
#include "Entry.hpp"
#include "FileTree.hpp"
#include "../Audio/AudioBuffer.hpp"
#include "../Audio/AudioFile.hpp"

using std::vector;
using std::unordered_map;
using std::pair;
using std::string;
using CATE::Entry;
using CATE::AudioFile;
using CATE::Point;
using CATE::PointCloud;

namespace CATE {

void Database::add_file(const string &path)
{
    Entry entry(path);
    Json json_object = to_json_entry(entry);
    db.push_back(json_object);
}

void Database::add_directory(const string &directory_path)
{
    vector<string> file_paths;
    CATE::get_nested_files(file_paths, directory_path);

    for (const auto &path : file_paths)
    {
        add_file(path);
    }
}

void Database::to_json_file(const string &path)
{
    std::ofstream file(path);
    file << std::setw(4) << db;
}

void Database::read_json_file(const string &path)
{
    std::ifstream ifstream(path);
    ifstream >> db;
}

void Database::convert_sample_rates(double new_sr)
{
    for (auto &buffer : files)
    {
        buffer.convert_sample_rate(new_sr);
    }
}

void Database::load_files()
{
    for (const auto &entry : db)
    {
        const string path = entry["path"];
        files.emplace_back(AudioFile(path));
    }
}

void Database::sliding_window_analysis(int bin_size, int frames_per_buffer, const string &output_path)
{
    FFT fft(bin_size, frames_per_buffer);
    vector<float> magspec(bin_size);
    int buffer_count = 0;

    for (auto b : files)
    {
        map<int, AudioBuffer> frames = segment_frames(b.data, frames_per_buffer);
        SpectralFeature spectral_feature(b.sample_rate, bin_size);

        for (auto it = frames.begin(); it != frames.end(); ++it)
        {
            int marker = it->first;
            AudioBuffer buffer = it->second;

            fft.fill(&buffer[0]);
            fft.compute();
            fft.get_magspec(magspec);

            float centroid = spectral_feature.centroid(magspec);
            float flatness = spectral_feature.flatness(magspec);

            db[buffer_count]["path"] = b.path;
            db[buffer_count]["centroid"].emplace_back(centroid);
            db[buffer_count]["flatness"].emplace_back(flatness);
            db[buffer_count]["markers"].emplace_back(marker);
        }

        ++buffer_count;
    }

    to_json_file(output_path);
}

PointCloud Database::create_point_cloud()
{
    PointCloud cloud;

    for (auto entry : db)
    {
        int num_segments = entry["markers"].size();

        for (auto i = 0; i < num_segments; ++i)
        {
            float centroid = entry["centroid"][i];
            float flatness = entry["flatness"][i];
            int marker = entry["markers"][i];
            string file_path = entry["path"];

            Point point {centroid, flatness, marker, file_path};

            cloud.points.emplace_back(point);
        }
    }

    return cloud;
}

} // CATE
