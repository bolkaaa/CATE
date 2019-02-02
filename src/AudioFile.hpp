#ifndef AUDIO_FILE_HPP
#define AUDIO_FILE_HPP

#include "AudioBuffer.hpp"

#include "../lib/sndfile.hh"

class AudioFile
{
public:
    AudioFile(const std::string &path);

    AudioFile(const std::string &path, int sample_rate, int channels,
	      int format); 

    void read(float *data);

    void write(float *data, unsigned long size);

    unsigned int channels() { return _channels; }

    unsigned long samples() { return _samples; }

    unsigned int sampleRate() { return _sampleRate; }

private:
    SndfileHandle file;
    unsigned int _channels;
    unsigned long _samples;
    unsigned int _sampleRate;
};

#endif
