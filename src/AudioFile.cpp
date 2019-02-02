#include "../lib/sndfile.hh"

#include "AudioFile.hpp"

AudioFile::AudioFile(const std::string &path)
{
    file = SndfileHandle(path);

    _sampleRate = file.samplerate();
    _channels = file.channels();
    _samples = file.frames();
}

AudioFile::AudioFile(const std::string &path, int sample_rate, int channels,
		     int format)
{
    file = SndfileHandle(path, SFM_WRITE, format, channels, sample_rate);
}


void AudioFile::read(float *data)
{
    unsigned long size = _channels * _samples;
    file.read(data, size);
}

void AudioFile::write(float *data, unsigned long size)
{
    file.write(data, size);
}
