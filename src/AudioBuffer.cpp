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

AudioBuffer::AudioBuffer()
    : data(vector<float>(1024))
{
}

AudioBuffer::AudioBuffer(uint32_t sz)
    : data(vector<float>(sz))
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
    uint64_t size = file.frames() * file.channels();
    data = vector<float>(size);
    file.read(&data[0], size);
    boost::filesystem::path p(path);

    fname = p.stem().string();
    sr = file.samplerate();
    chan = file.channels();
}

void AudioBuffer::write(const string &path, uint32_t sample_rate,
                        uint8_t channels, uint8_t format)
{
    SndfileHandle file(path, SFM_WRITE, format, channels, sample_rate);
    file.write(&data[0], data.size());
}

void AudioBuffer::to_file(const std::string &path)
{
    std::ofstream file(path);

    for (uint64_t i = 0; i < data.size(); ++i)
    {
        file << data[i] << "\n";
    }
}

void AudioBuffer::convert_sample_rate(uint32_t new_sr)
{
    double sr_ratio = new_sr / sr;
    vector<float> out(sr_ratio * data.size());
    SRC_DATA conv;

    conv.data_in = &data[0];
    conv.data_out = &out[0];
    conv.input_frames = (data.size() / chan);
    conv.output_frames = ((sr_ratio * data.size()) / chan);
    conv.src_ratio = sr_ratio;

    src_simple(&conv, SRC_SINC_BEST_QUALITY, chan);

    data = out;
}
