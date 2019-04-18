#include "AudioRecorder.hpp"
#include "AudioFile.hpp"

namespace CATE {

AudioRecorder::AudioRecorder()
    : buffer(AudioBuffer(max_recording_size))
{
}

void AudioRecorder::write(float sample)
{
    if (record_pos < max_recording_size)
    {
        buffer[record_pos] = sample;
        ++record_pos;
    }
}

void AudioRecorder::save(std::string output_path, int num_channels, float sample_rate)
{
    buffer.resize(record_pos);
    AudioFile file_buffer;
    file_buffer.write(buffer,
                      num_channels,
                      sample_rate / num_channels,
                      output_path);
}

} // CATE
