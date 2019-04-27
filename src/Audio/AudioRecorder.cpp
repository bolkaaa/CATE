#include "AudioRecorder.hpp"
#include "AudioFile.hpp"

namespace CATE {

AudioRecorder::AudioRecorder(float sample_rate)
        : sample_rate(sample_rate),
          max_recording_size(sample_rate * 60.0f * max_recording_minutes),
          buffer(AudioBuffer(max_recording_size))
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

void AudioRecorder::save(const std::string &output_path, int num_channels, float sample_rate)
{
    AudioBuffer output_buffer(buffer.begin(), buffer.begin() + record_pos);
    AudioFile file_buffer;
    file_buffer.write(output_buffer,
                      num_channels,
                      sample_rate / num_channels,
                      output_path);
}

} // CATE
