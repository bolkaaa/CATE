#ifndef AUDIO_BUFFER_HPP
#define AUDIO_BUFFER_HPP

#include <vector>
#include <string>

class AudioBuffer
{
public:
    AudioBuffer();

    AudioBuffer(float *data, unsigned int sz);

    AudioBuffer(const std::string &path);

    unsigned long size() { return data.size(); }

    void write(const std::string &path, int sample_rate, int channels, int format);

    float& operator[](unsigned int index);

private:
    void read(const std::string &path);

    std::vector<float> data;
};

#endif
