#ifndef AUDIO_BUFFER_HPP
#define AUDIO_BUFFER_HPP

#include <string>

class AudioBuffer
{
public:
    AudioBuffer(const std::string &path);

    ~AudioBuffer();

    void write(const std::string &path, int sample_rate, int channels, int format);

    unsigned int size() { return sz; }

    float& operator[](unsigned int index);

    float* begin() { return &data[0]; }

    float* end() { return &data[sz]; }

    float* d() { return data; }

private:
    void read(const std::string &path);

    float *data;
    unsigned int sz;
};

#endif
