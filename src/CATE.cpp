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
typedef portaudio::System AudioBackend;

class AudioParameters
{
public:
    AudioParameters(AudioBackend &backend)
	: input_channels(2), output_channels(2), sample_rate(44100),
	  frames_per_buffer(256)
    {
	input = portaudio::DirectionSpecificStreamParameters(
	    backend.defaultInputDevice(),
	    2,
	    portaudio::FLOAT32,
	    false,
	    backend.defaultInputDevice().defaultLowInputLatency(),
	    nullptr);

	output = portaudio::DirectionSpecificStreamParameters(
	    backend.defaultOutputDevice(),
	    output_channels,
	    portaudio::FLOAT32,
	    false,
	    backend.defaultOutputDevice().defaultLowOutputLatency(),
	    nullptr);

	stream = portaudio::StreamParameters(
	    input,
	    output,
	    sample_rate,
	    frames_per_buffer,
	    paClipOff);
    }

    AudioParameters(AudioBackend &backend, unsigned long sample_rate,
		     unsigned long frames_per_buffer,
		     unsigned int input_channels, unsigned int output_channels)
	: input_channels(input_channels), output_channels(output_channels),
	  sample_rate(sample_rate), frames_per_buffer(frames_per_buffer)
    {
	input = portaudio::DirectionSpecificStreamParameters(
	    backend.defaultInputDevice(),
	    input_channels,
	    portaudio::FLOAT32,
	    false,
	    backend.defaultInputDevice().defaultLowInputLatency(),
	    nullptr);

	output = portaudio::DirectionSpecificStreamParameters(
	    backend.defaultOutputDevice(),
	    output_channels,
	    portaudio::FLOAT32,
	    false,
	    backend.defaultOutputDevice().defaultLowOutputLatency(),
	    nullptr);

	stream = portaudio::StreamParameters(
	    input,
	    output,
	    sample_rate,
	    frames_per_buffer,
	    paClipOff);
    }

    portaudio::DirectionSpecificStreamParameters get_input() { return input; }

    portaudio::DirectionSpecificStreamParameters get_output() { return output; }

    portaudio::StreamParameters get_stream() { return stream; }

private:
    unsigned int input_channels;
    unsigned int output_channels;
    unsigned long sample_rate;
    unsigned long frames_per_buffer;
    portaudio::DirectionSpecificStreamParameters input;
    portaudio::DirectionSpecificStreamParameters output;
    portaudio::StreamParameters stream;
};

int main(int argc, char *argv[])
{
    portaudio::AutoSystem auto_system;
    AudioBackend &audio_backend = portaudio::System::instance();
    AudioParameters audio_parameters(audio_backend);

    /* Load an audio file into an AudioBuffer container and pass it to the 
       Synth object. */
    std::string audio_file_path = argv[1];
    AudioBuffer buffer(audio_file_path);
    Synth synth(buffer);

    /* Create a PortAudio stream for the CATE callback and start it on a new thread. */
    Stream stream(audio_parameters.get_stream(), synth, &Synth::process);
    std::thread audio_thread(&portaudio::MemFunCallbackStream<Synth>::start, &stream);
    audio_thread.detach();

    while (1)
    {

    }

    stream.stop();
    stream.close();
    audio_backend.terminate();

    return 0;
}
