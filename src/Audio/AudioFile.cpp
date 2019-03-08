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

AudioFile::AudioFile()
        : sr(0), chan(0)
{
}

void AudioFile::read()
{
    SndfileHandle file(path);
    auto size = file.frames() * file.channels();
    AudioBuffer b(size);
    file.read(&b[0], size);

    data = b;
    sr = file.samplerate();
    chan = file.channels();
}

void AudioFile::write(const AudioBuffer &buffer, const string &path, int format)
{
    SndfileHandle file(path, SFM_WRITE, format, chan, static_cast<int>(sr));
    file.write(&data[0], static_cast<sf_count_t>(data.size()));
}

void AudioFile::convert_sample_rate(double new_sr)
{
    double sr_ratio = new_sr / sr;
    AudioBuffer out(sr_ratio * data.size());
    SRC_DATA conv;

    conv.data_in = &data[0];
    conv.data_out = &out[0];
    conv.input_frames = (data.size() / chan);
    conv.output_frames = (sr_ratio * data.size()) / chan;
    conv.src_ratio = sr_ratio;

    src_simple(&conv, SRC_SINC_BEST_QUALITY, chan);

    data = out;
}

} // CATE
