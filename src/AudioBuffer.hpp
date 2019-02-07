#ifndef AUDIO_BUFFER_HPP
#define AUDIO_BUFFER_HPP

#include <vector>
#include <string>
#include <iostream>
#include <boost/filesystem.hpp>

#include "sndfile.hh"
#include "samplerate.h"

template <class T>
constexpr T ms_to_samp(unsigned int ms, unsigned long sr)
/* Conversion millisecond time period to sample number. */
{
    return (ms / 1000.) * sr;
}

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
	double sr_ratio = new_sr / sr;
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

    void segment(std::vector<AudioBuffer<T>> &segments, unsigned int grain_size, unsigned long sample_rate)
    /* Naive version with no windowing yet. */
    {
	unsigned int samples = ms_to_samp<T>(grain_size, sample_rate);

	for (auto it = data.begin(); it < data.end(); it += samples)
	{
	    std::vector<T> grain(it, it + samples);
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
    std::string filename;
    unsigned int sr;
    unsigned int channels;
    
};

#endif
