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
          fft(audio_settings)
{
}

void Corpus::add_directory(const string &directory_path)
{
    vector<string> file_paths;
    get_nested_files(file_paths, directory_path);

    for (const auto &path : file_paths)
    {
        AudioFile file(path);
        files.insert(pair<string, AudioFile>(path, file));
    }
}

void Corpus::set_file(const string &new_file_path)
{
    data_path = new_file_path;
}

void Corpus::write_file()
{
    std::ofstream file(data_path);
    file << std::setw(4) << db;
}

void Corpus::read_file()
{
    std::ifstream ifstream(data_path);
    ifstream >> db;
}

void Corpus::convert_sample_rates(double new_sr)
{
    for (auto &file : files)
    {
        file.second.convert_sample_rate(new_sr);
    }
}

vector<float> Corpus::calculate_frame_spectrum(const pair<int, AudioBuffer> &frame)
{
    AudioBuffer buffer(frame.second);

    fft.fill(&buffer[0]);
    fft.compute_spectrum();
    fft.compute_magspec();
    vector<float> magspec = fft.get_magspec();

    return magspec;
}

void Corpus::load_audio_from_db()
{
    for (auto entry : db)
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

        for (auto &frame : frames)
        {
            vector<float> magspec = calculate_frame_spectrum(frame);
            Feature feature(audio_settings.get_bin_size());
            Json unit;

            unit["path"] = file.first;
            unit["marker"] = frame.first;
            unit["centroid"] = feature.centroid(magspec);
            unit["flatness"] = feature.flatness(magspec);

            db.emplace_back(unit);
        }
    }
}

PointCloud Corpus::create_point_cloud()
{
    PointCloud cloud;

    for (auto entry : db)
    {
        int marker = entry["marker"];
        string file_path = entry["path"];
        float centroid = entry["centroid"];
        float flatness = entry["flatness"];

        Point point {marker, file_path, centroid, flatness};

        cloud.points.emplace_back(point);
    }

    return cloud;
}

bool Corpus::has_data()
{
    return !db.empty();
}

} // CATE
