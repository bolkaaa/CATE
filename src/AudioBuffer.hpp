#ifndef AUDIO_BUFFER_HPP
#define AUDIO_BUFFER_HPP

#include <vector>
#include <string>
#include <iostream>
#include <boost/filesystem.hpp>

#include "../lib/sndfile.hh"

template <class T>
class AudioBuffer
{
public:
    AudioBuffer<T>(const std::string &path)
    {
	read(path);
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

    std::string get_filename() { return filename; }

private:
    void read(const std::string &path)
    {
	SndfileHandle file(path);
	unsigned long size = file.frames() * file.channels();
	data = std::vector<T>(size);
	file.read(&data[0], size);
	boost::filesystem::path p(path);
	filename = p.stem().string();
    }

    std::vector<T> data;
    std::string filename;
};

#endif
