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

#include "FileTree.hpp"
#include "../Audio/AudioBuffer.hpp"
#include "../Audio/AudioFile.hpp"
#include "../Analysis/FFT.hpp"
#include "../Analysis/SpectralFeature.hpp"
#include "Entry.hpp"

using std::vector;
using std::unordered_map;
using std::string;
using Json = nlohmann::json;
using CATE::AudioBuffer;
using CATE::FFT;
using CATE::SpectralFeature;

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

    /* Add all files deeper than specified directory to the database.
       TODO: Add some way of ignoring non-audio files. */
    void add_directory(const string &directory_path);

    /* Iterate over JSON database and load audio files into <buffers>. */
    void load_buffers_from_db();

    vector<AudioBuffer> segment(const AudioBuffer &source, int frame_size)
    {
        vector<AudioBuffer> segments;
        auto n = source.size();
        auto remaining_space = n % frame_size;

        for (auto it = source.begin(); it != (source.end() - remaining_space); it += frame_size)
        {
            AudioBuffer segment_data(it, it + frame_size);
            segments.emplace_back(segment_data);
        }

        if (!segments.empty())
        {
            return segments;
        }

        return vector<AudioBuffer>(0);
    }

    void sliding_window_analysis(int bin_size, int frames_per_buffer)
    {
        FFT fft(bin_size, frames_per_buffer);
        vector<float> magspec(bin_size);

        for (auto b : buffers)
        {
            vector<AudioBuffer> segments = segment(b.second.data, frames_per_buffer);
            SpectralFeature spectral_feature(b.second.sr, bin_size);

            for (auto segment : segments)
            {
                float *data = &segment[0];
                fft.fill(data);
                fft.compute();
                fft.get_magspec(magspec);
                float centroid = spectral_feature.centroid(magspec);
                float flatness = spectral_feature.flatness(magspec);
                std::cout << centroid << ", " << flatness << " \n";
            }
        }
    }

    /* Clear all buffers in database. */
    void clear_buffers();

    /* Convert all buffers in database to new sample rate. */
    void convert_sample_rates(double new_sr);

    /* Get the size of the database. */
    int size() const
    { return static_cast<int>(db.size()); }

    /* Get keys from buffers map as a vector of strings. */
    vector<string> get_paths() const;

    /* Get values from buffers map as a vector of AudioBuffers. */
    vector<AudioFile> get_files() const;

    /* Check if a particular file exists in the database. */
    bool buffer_exists(const string &key);

    /* Save the JSON database to a JSON file, with pretty printing. */
    void to_json_file(const string &path);

    /* Get a particular file from the database, indexed by filename. */
    AudioFile &operator[](const string &file_path)
    { return buffers[file_path]; }

    /* Output the database as a stream. */
    friend std::ostream &operator<<(std::ostream &os, const Database &database);

private:
    /* JSON data object, storing file paths, segmentation frame markers and
     * analysis data. */
    Json db;

    /* Hash map of audio files indexed by file names. */
    unordered_map<string, AudioFile> buffers;
};

} // CATE

#endif
