#include <iostream>

#include "sndfile.hh"

#include "AudioBuffer.hpp"

AudioBuffer::AudioBuffer()
{
}

AudioBuffer::AudioBuffer(const std::string &path)
{
    read(path);
}

void AudioBuffer::read(const std::string &path)
{
    SndfileHandle file(path);
    unsigned long size = file.frames() * file.channels();
    data = std::vector<float>(size);
    file.read(&data[0], size);
}

void AudioBuffer::write(const std::string &path, int sample_rate, int channels, int format)
{
    SndfileHandle file(path, SFM_WRITE, format, channels, sample_rate);
    file.write(&data[0], data.size());
}


float& AudioBuffer::operator[](unsigned int i)
{
    return data[i];
}
