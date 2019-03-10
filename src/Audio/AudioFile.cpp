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

#include "AudioFile.hpp"

using std::vector;
using std::string;
using Path = boost::filesystem::path;

namespace CATE {

AudioFile::AudioFile(const string &path)
        : path(path)
{
    read();
}

void AudioFile::read()
{
    SndfileHandle file(path);
    auto size = file.frames() * file.channels();
    AudioBuffer buffer(size);
    file.read(&buffer[0], size);

    data = buffer;
    sample_rate = file.samplerate();
    channels = file.channels();
}

void AudioFile::write(const AudioBuffer &buffer, const string &path, int format)
{
    SndfileHandle file(path, SFM_WRITE, format, channels, static_cast<int>(sample_rate));
    file.write(&data[0], static_cast<sf_count_t>(data.size()));
}

void AudioFile::convert_sample_rate(double new_sample_rate)
{
    double sr_ratio = new_sample_rate / sample_rate;
    AudioBuffer output(sr_ratio * data.size());
    SRC_DATA conv;

    conv.data_in = &data[0];
    conv.data_out = &output[0];
    conv.input_frames = (data.size() / channels);
    conv.output_frames = (sr_ratio * data.size()) / channels;
    conv.src_ratio = sr_ratio;

    src_simple(&conv, SRC_SINC_BEST_QUALITY, channels);

    data = output;
}

} // CATE
