#ifndef AUDIO_BUFFER_HPP
#define AUDIO_BUFFER_HPP

#include <vector>
#include <string>
#include <iostream>
#include <boost/filesystem.hpp>

#include "sndfile.hh"
#include "samplerate.h"

template <class T>
class AudioBuffer
{
public:
    AudioBuffer<T>()
    {
    }

    AudioBuffer<T>(const std::string &path)
    {
	read(path);
    }

    AudioBuffer<T>(std::vector<T> data)
	: data(data)
    {
    }

    void write(const std::string &path, int sample_rate, int channels, int format)
    {
	SndfileHandle file(path, SFM_WRITE, format, channels, sample_rate);
	file.write(&data[0], data.size());
    }

    T& operator[](unsigned long i)
    {
	return data[i];
    }

    unsigned long size() { return data.size(); }

    unsigned int chan() { return channels; }

    std::string get_filename() { return filename; }

    void convert_sample_rate(unsigned int new_sr)
    {
	double sr_ratio = static_cast<double> (new_sr / sr);
	std::vector<T> out(sr_ratio * data.size());
	SRC_DATA conv;

	conv.data_in = &data[0];
	conv.data_out = &out[0];
	conv.input_frames = (data.size() / channels);
	conv.output_frames = ((sr_ratio * data.size()) / channels);
	conv.src_ratio = sr_ratio;

	src_simple(&conv, SRC_SINC_BEST_QUALITY, channels);

	data = out;
    }

    void segment(std::vector<AudioBuffer<T>> &segments, unsigned long grain_size)
    /* Naive version with no windowing. */
    {
	for (auto it = data.begin(); it < data.end(); it += grain_size)
	{
	    std::vector<T> grain(it, it + grain_size);
	    AudioBuffer<T> buffer(grain);
	    segments.push_back(buffer);
	}
    }


private:
    void read(const std::string &path)
    {
	SndfileHandle file(path);
	unsigned long size = file.frames() * file.channels();
	data = std::vector<T>(size);
	file.read(&data[0], size);
	boost::filesystem::path p(path);
	filename = p.stem().string();
	sr = file.samplerate();
	channels = file.channels();
    }

    std::vector<T> data;
    unsigned int channels;
    unsigned int sr;
    std::string filename;
};

#endif
