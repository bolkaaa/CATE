#include <iostream>
#include <thread>
#include <vector>

#include "../lib/portaudio.h"
#include "../lib/PortAudioCpp.hxx"
#include "../lib/sndfile.h"
#include "../lib/sndfile.hh"

#include "AudioBuffer.hpp"
#include "Synth.hpp"

typedef portaudio::MemFunCallbackStream<Synth> Stream;

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
    Synth synth(buffer);

    /* Create a PortAudio stream for the CATE callback and start it on a new thread. */
    Stream stream(stream_parameters.stream, synth, &Synth::process);
    std::thread audio_thread(&portaudio::MemFunCallbackStream<Synth>::start, &stream);
    audio_thread.detach();

    while (1)
    {

    }

    stream.stop();
    stream.close();
    system.terminate();

    return 0;
}
