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

template <class T>
class AudioBuffer
{
public:
    AudioBuffer<T>();

    AudioBuffer<T>(uint32_t sz);

    /* Instantiate with file read from path. */
    AudioBuffer<T>(const string &path);

    /* Instantiate with vector of data. */
    AudioBuffer<T>(const vector<T> &data);

    /* Write buffer object to output file. */
    void write(const string &path, uint32_t sample_rate, uint8_t channels,
               uint8_t format);

    /* Write sample data to line-separated text file. */
    void to_file(const string &path);

    /* Get indexed sample value from private samples vector. */
    T& operator[](const uint32_t i) { return data[i]; }

    /* Get indexed sample value from private samples vector (const version). */
    const T& operator[](const uint32_t i) const { return data[i]; }

    /* Output sample values through stream. */
    template <class>
    friend std::ostream& operator<<(std::ostream &os, const AudioBuffer &b);

    /* Get sample rate of buffer. */
    uint32_t sample_rate() { return sr; }

    /* Get number of samples in buffer. */
    uint64_t size() const { return data.size(); }

    /* Get number of channels in buffer. */
    uint8_t channels() { return chan; }

    /* Get filename associated with buffer. */
    string filename() { return fname; }

    /* Interpolate sample rate of buffer to new sample rate. */
    void convert_sample_rate(uint32_t new_sr);

private:
    /* Read audio file from path. */
    void read(const string &path);

    string fname;
    uint32_t sr;
    uint8_t chan;

protected:
    vector<T> data;

};

template <class T>
AudioBuffer<T>::AudioBuffer()
    : data(vector<T>(1024))
{
}

template <class T>
AudioBuffer<T>::AudioBuffer(uint32_t sz)
    : data(vector<T>(sz))
{
}

template <class T>
AudioBuffer<T>::AudioBuffer(const string &path)
{
    read(path);
}

template <class T>
AudioBuffer<T>::AudioBuffer(const vector<T> &data)
    : data(data)
{
}

template <class T>
void AudioBuffer<T>::read(const string &path)
{
    SndfileHandle file(path);
    uint64_t size = file.frames() * file.channels();
    data = vector<T>(size);
    file.read(&data[0], size);
    boost::filesystem::path p(path);

    fname = p.stem().string();
    sr = file.samplerate();
    chan = file.channels();
}

template <class T>
void AudioBuffer<T>::write(const string &path, uint32_t sample_rate,
                           uint8_t channels, uint8_t format)
{
    SndfileHandle file(path, SFM_WRITE, format, channels, sample_rate);
    file.write(&data[0], data.size());
}

template <class T>
void AudioBuffer<T>::to_file(const std::string &path)
{
    std::ofstream file(path);

    for (uint64_t i = 0; i < data.size(); ++i)
    {
        file << data[i] << "\n";
    }
}


template <class T>
void AudioBuffer<T>::convert_sample_rate(uint32_t new_sr)
{
    double sr_ratio = new_sr / sr;
    vector<T> out(sr_ratio * data.size());
    SRC_DATA conv;

    conv.data_in = &data[0];
    conv.data_out = &out[0];
    conv.input_frames = (data.size() / chan);
    conv.output_frames = ((sr_ratio * data.size()) / chan);
    conv.src_ratio = sr_ratio;

    src_simple(&conv, SRC_SINC_BEST_QUALITY, chan);

    data = out;
}

#endif
