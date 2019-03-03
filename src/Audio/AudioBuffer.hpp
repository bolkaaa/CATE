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

#ifndef AUDIO_BUFFER_HPP
#define AUDIO_BUFFER_HPP

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>

#include "sndfile.hh"
#include "samplerate.h"

using std::vector;
using std::string;

/*
  The AudioBuffer class is intended to be a generalisation of a collection of
  audio sample data, with associated member functions for working with it, such
  as reading from audio files. At the moment, the data itself is stored in a
  std::vector, with a number of public wrapper functions for manipulating it.
  Other classes such as RingBuffer will inherit from the base AudioBuffer class.
 */

class AudioBuffer
{
public:
    explicit AudioBuffer() = default;

    explicit AudioBuffer(int sz);

    /* Instantiate with file read from path. */
    explicit AudioBuffer(const string &path);

    /* Instantiate with vector of data. */
    explicit AudioBuffer(const vector<float> &data);

    /* Read audio file from path. */
    void read(const string &path);

    /* Write buffer object to output file. */
    void write(const string &path, double sample_rate, int channels,
               int format);

    /* Write sample data to line-separated text file. */
    void to_file(const string &path);

    /* Get sample rate of buffer. */
    double sample_rate() { return sr; }

    /* Get number of samples in buffer. */
    int size() const { return static_cast<int>(data.size()); }

    /* Get number of channels in buffer. */
    int channels() { return chan; }

    /* Get filename associated with buffer. */
    string filename() { return fname; }

    /* Interpolate sample rate of buffer to new sample rate. */
    void convert_sample_rate(double new_sr);

    vector<float> get_data() { return data; }

    float& operator[](const int i) { return data[i]; }

    const float& operator[](const int i) const { return data[i]; }

    void operator=(const AudioBuffer &b) { data = b.data; }

private:
    string fname;
    double sr;
    int chan;

protected:
    vector<float> data;

};

#endif

