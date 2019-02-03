#include <iostream>
#include <thread>
#include <vector>

#include "../lib/portaudio.h"
#include "../lib/PortAudioCpp.hxx"
#include "../lib/sndfile.h"
#include "../lib/sndfile.hh"

#include "AudioBuffer.hpp"

class CATE
{
public:
    CATE(AudioBuffer buffer)
	: buffer(buffer), index(0)
    {
    }

    int process(const void *input, void *output, unsigned long frames_per_buffer, 
		const PaStreamCallbackTimeInfo *time_info, PaStreamCallbackFlags status_flags)
    {
	float **out = static_cast<float**>(output);

	for (unsigned int i = 0; i < frames_per_buffer; ++i)
	{
	    /* Left Channel */
	    out[0][i] = buffer[index];
	    ++index;

	    /* Right Channel */
	    out[1][i] = buffer[index];
	    ++index;

	    if (index >= buffer.size())
	    {
		index -= buffer.size();
	    }
	}

	return paContinue;
    }

    AudioBuffer buffer;
    unsigned int index;

private:
};

class StreamParameters
{
public:
    StreamParameters(portaudio::System &sys, unsigned long sample_rate,
		     unsigned long frames_per_buffer,
		     unsigned int input_channels, unsigned int output_channels)
    {
	input = portaudio::DirectionSpecificStreamParameters(
	    sys.defaultInputDevice(),
	    input_channels,
	    portaudio::FLOAT32,
	    false,
	    sys.defaultInputDevice().defaultLowInputLatency(),
	    nullptr);

	output = portaudio::DirectionSpecificStreamParameters(
	    sys.defaultOutputDevice(),
	    output_channels,
	    portaudio::FLOAT32,
	    false,
	    sys.defaultOutputDevice().defaultLowOutputLatency(),
	    nullptr);

	stream = portaudio::StreamParameters(
	    input,
	    output,
	    sample_rate,
	    frames_per_buffer,
	    paClipOff);
    }

    portaudio::StreamParameters stream;

private:
    portaudio::DirectionSpecificStreamParameters input;
    portaudio::DirectionSpecificStreamParameters output;
};


int main(int argc, char *argv[])
{
    unsigned long sample_rate = 44100;
    unsigned long frames_per_buffer = 256;
    unsigned int input_channels = 2;
    unsigned int output_channels = 2;

    portaudio::AutoSystem auto_sys;
    portaudio::System &system = portaudio::System::instance();

    StreamParameters stream_parameters(
	system,
	sample_rate,
	frames_per_buffer,
	input_channels,
	output_channels);

    /* Load an audio file into an AudioBuffer container and pass it to the 
       CATE object. */
    std::string audio_file_path = argv[1];
    AudioBuffer buffer(audio_file_path);
    CATE cate(buffer);

    /* Create a PortAudio stream for the CATE callback and start it on a new thread. */
    portaudio::MemFunCallbackStream<CATE> stream(stream_parameters.stream, cate, &CATE::process);
    std::thread audio_thread(&portaudio::MemFunCallbackStream<CATE>::start, &stream);
    audio_thread.detach();

    while (1)
    {

    }

    stream.stop();
    stream.close();
    system.terminate();

    return 0;
}
