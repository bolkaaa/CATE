#include <iostream>

#include "sndfile.h"

#include "AudioBuffer.hpp"
#include "AudioFile.hpp"

AudioBuffer::AudioBuffer(const std::string &path)
{
    read(path);
}

AudioBuffer::~AudioBuffer()
{
    free(data);
}

void AudioBuffer::read(const std::string &path)
{
    AudioFile file(path);
    sz = file.samples() * file.channels();
    data = new float[sz];
    file.read(data);
}

void AudioBuffer::write(const std::string &path, int sample_rate, int channels, int format)
{
    AudioFile file(path, sample_rate, channels, format);
    file.write(data, sz);
}


float& AudioBuffer::operator[](unsigned int index)
{
    return data[index];
}
