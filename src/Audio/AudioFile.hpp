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

#ifndef AUDIO_FILE
#define AUDIO_FILE

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include <boost/filesystem.hpp>
#include "sndfile.hh"

#include "AudioBuffer.hpp"

using std::vector;
using std::string;

/* The AudioFile class is a simple abstraction  of an audio file, containing the
 * audio data and file information along with member functions for manipulating
 * them. */

namespace CATE {

class AudioFile
{
public:
    explicit AudioFile() = default;

    explicit AudioFile(const string &path);

    /* Read audio file from path. */
    void read(const string &path);

    /* Write data to output WAV file. */
    void write(int channels, float sample_rate, const string &path);


    AudioBuffer data;
    string path;
    float sample_rate;
    int channels;
};

} // CATE

#endif
