#include <iostream>
#include <thread>

#include "AudioFile.hpp"
#include "AudioBuffer.hpp"

#include "../lib/portaudio.h"
#include "../lib/PortAudioCpp.hxx"
#include "../lib/sndfile.h"
#include "../lib/sndfile.hh"

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
	assert(output != nullptr);

	float **out = static_cast<float**>(output);

	for (unsigned int i = 0; i < frames_per_buffer; ++i)
	{
	    out[0][i] = buffer[index];
	    index += 1;

	    out[1][i] = buffer[index];
	    index += 1;

	    if (index >= buffer.size())
		index -= buffer.size();
	}

	return paContinue;
    }

    AudioBuffer buffer;
    unsigned int index;

private:
};

int main(int argc, char *argv[])
{
    std::string input_path = argv[1];
    unsigned long sample_rate = 48000;
    unsigned long frames_per_buffer = 256;
    unsigned int channels = 2;
    AudioBuffer buffer(input_path);
    CATE cate(buffer);

    // Audio Setup
    portaudio::AutoSystem autoSys;
    portaudio::System &sys = portaudio::System::instance();

    // Output Parameters
    portaudio::DirectionSpecificStreamParameters outParams(
	sys.defaultOutputDevice(),
	channels,
	portaudio::FLOAT32,
	false,
	sys.defaultOutputDevice().defaultLowOutputLatency(),
	NULL);

    // Stream parameters
    portaudio::StreamParameters params(
	portaudio::DirectionSpecificStreamParameters::null(),
	outParams,
	sample_rate,
	frames_per_buffer,
	paClipOff);

    portaudio::MemFunCallbackStream<CATE> stream(params, cate, &CATE::process);

    stream.start();

    sys.sleep(1000 * ((buffer.size() / sample_rate) / 2));

    stream.stop();

    stream.close();

    sys.terminate();

    return 0;
}
