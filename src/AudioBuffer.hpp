#ifndef AUDIO_BUFFER_HPP
#define AUDIO_BUFFER_HPP

#include <vector>
#include <string>
#include <iostream>
#include <boost/filesystem.hpp>

#include "sndfile.hh"
#include "samplerate.h"

template <class T>
constexpr T ms_to_samp(uint32_t ms, uint32_t sr)
/* Convert millisecond time period to sample number. */
{
    return (ms / 1000.) * sr;
}

template <class T>
class AudioBuffer
{
public:
    /* Default constructor. */
    AudioBuffer<T>();

    /* Instantiate with file read from path. */
    AudioBuffer<T>(const std::string &path);

    /* Instantiate with vector of sample data. */
    AudioBuffer<T>(std::vector<T> data);

    /* Write buffer object to output file. */
    void write(const std::string &path, uint32_t sample_rate, uint8_t channels,
	       uint8_t format);

    /* Get indexed sample value from private samples vector. */
    T& operator[](uint32_t i) { return data[i]; }

    /* Get sample rate of buffer. */
    uint32_t sample_rate() { return sr; }

    /* Get number of samples in buffer. */
    uint64_t size() { return data.size(); }

    /* Get number of channels in buffer. */
    uint8_t channels() { return chan; }

    /* Get filename associated with buffer. */
    std::string filename() { return fname; }

    /* Interpolate sample rate of buffer to new sample rate. */
    void convert_sample_rate(uint32_t new_sr);

    /* Output a vector of buffers from original buffer
       (naive version with no windowing yet). */
    void segment(std::vector<AudioBuffer<T>> &segments,
		 uint16_t grain_size, uint32_t sample_rate);

private:
    /* Read audio file from path. */
    void read(const std::string &path);

    std::vector<T> data;
    std::string fname;
    uint32_t sr;
    uint8_t chan;
};

template <class T>
AudioBuffer<T>::AudioBuffer()
{
}

template <class T>
AudioBuffer<T>::AudioBuffer(const std::string &path)
{
    read(path);
}

template <class T>
AudioBuffer<T>::AudioBuffer(std::vector<T> data)
    : data(data)
{
}

template <class T>
void AudioBuffer<T>::read(const std::string &path)
{
    SndfileHandle file(path);
    uint64_t size = file.frames() * file.channels();
    data = std::vector<T>(size);
    file.read(&data[0], size);
    boost::filesystem::path p(path);

    fname = p.stem().string();
    sr = file.samplerate();
    chan = file.channels();
}

template <class T>
void AudioBuffer<T>::write(const std::string &path, uint32_t sample_rate,
			   uint8_t channels, uint8_t format)
{
    SndfileHandle file(path, SFM_WRITE, format, channels, sample_rate);
    file.write(&data[0], data.size());
}


template <class T>
void AudioBuffer<T>::convert_sample_rate(uint32_t new_sr)
{
    double sr_ratio = new_sr / sr;
    std::vector<T> out(sr_ratio * data.size());
    SRC_DATA conv;

    conv.data_in = &data[0];
    conv.data_out = &out[0];
    conv.input_frames = (data.size() / chan);
    conv.output_frames = ((sr_ratio * data.size()) / chan);
    conv.src_ratio = sr_ratio;

    src_simple(&conv, SRC_SINC_BEST_QUALITY, chan);

    data = out;
}

template <class T>
void AudioBuffer<T>::segment(std::vector<AudioBuffer<T> > &segments, uint16_t grain_size, uint32_t sample_rate)
    
{
    uint32_t samples = ms_to_samp<T>(grain_size, sample_rate);

    for (auto it = data.begin(); it < data.end(); it += samples)
    {
	std::vector<T> grain(it, it + samples);
	AudioBuffer<T> buffer(grain);
	segments.push_back(buffer);
    }
}

#endif
