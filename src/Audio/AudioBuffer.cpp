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
#include <string>
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>

#include "sndfile.hh"
#include "samplerate.h"

#include "AudioBuffer.hpp"

using std::vector;
using std::string;

AudioBuffer::AudioBuffer(int sz, const string &fname, double sr, int chan)
    : fname(fname), sr(sr), chan(chan)
{
}

AudioBuffer::AudioBuffer(const string &path)
{
    read(path);
}

AudioBuffer::AudioBuffer(const vector<float> &data)
    : data(data)
{
}

void AudioBuffer::read(const string &path)
{
    SndfileHandle file(path);
    auto size = file.frames() * file.channels();
    data = vector<float>(size);
    file.read(&data[0], size);
    boost::filesystem::path p(path);

    fname = p.stem().string();
    sr = file.samplerate();
    chan = file.channels();
}

void AudioBuffer::write(const string &path, double sample_rate,
                        int channels, int format)
{
    SndfileHandle file(path, SFM_WRITE, format, channels, static_cast<int>(sample_rate));
    file.write(&data[0], static_cast<sf_count_t>(data.size()));
}

void AudioBuffer::to_file(const string &path)
{
    std::ofstream file(path);

    for (float sample_value : data)
    {
        file << sample_value << "\n";
    }
}

void AudioBuffer::convert_sample_rate(double new_sr)
{
    double sr_ratio = new_sr / sr;
    vector<float> out(static_cast<unsigned long>(sr_ratio * data.size()));
    SRC_DATA conv;

    conv.data_in = &data[0];
    conv.data_out = &out[0];
    conv.input_frames = (data.size() / chan);
    conv.output_frames = static_cast<long>((sr_ratio * data.size()) / chan);
    conv.src_ratio = sr_ratio;

    src_simple(&conv, SRC_SINC_BEST_QUALITY, chan);

    data = out;
}

vector<AudioBuffer> AudioBuffer::segment(int frame_size)
{
    vector<AudioBuffer> segments;
    auto n = data.size();
    auto remaining_space = n % frame_size;

    for (auto it = data.begin(); it != (data.end() - remaining_space); it += frame_size)
    {
        vector<float> segment_data(it, it + frame_size);
        AudioBuffer buffer(segment_data);
        segments.emplace_back(buffer);
    }

    return segments;
}
