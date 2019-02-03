#include "AudioParameters.hpp"

AudioParameters::AudioParameters(portaudio::System &system)
    : input_channels(2), output_channels(2), sample_rate(44100),
      frames_per_buffer(256)
{
    input = portaudio::DirectionSpecificStreamParameters(
	system.defaultInputDevice(),
	2,
	portaudio::FLOAT32,
	false,
	system.defaultInputDevice().defaultLowInputLatency(),
	nullptr);

    output = portaudio::DirectionSpecificStreamParameters(
	system.defaultOutputDevice(),
	output_channels,
	portaudio::FLOAT32,
	false,
	system.defaultOutputDevice().defaultLowOutputLatency(),
	nullptr);

    stream = portaudio::StreamParameters(
	input,
	output,
	sample_rate,
	frames_per_buffer,
	paClipOff);
}

AudioParameters::AudioParameters(portaudio::System &system, unsigned long sample_rate,
				 unsigned long frames_per_buffer,
				 unsigned int input_channels, unsigned int output_channels)
    : input_channels(input_channels), output_channels(output_channels),
      sample_rate(sample_rate), frames_per_buffer(frames_per_buffer)
{
    input = portaudio::DirectionSpecificStreamParameters(
	system.defaultInputDevice(),
	input_channels,
	portaudio::FLOAT32,
	false,
	system.defaultInputDevice().defaultLowInputLatency(),
	nullptr);

    output = portaudio::DirectionSpecificStreamParameters(
	system.defaultOutputDevice(),
	output_channels,
	portaudio::FLOAT32,
	false,
	system.defaultOutputDevice().defaultLowOutputLatency(),
	nullptr);

    stream = portaudio::StreamParameters(
	input,
	output,
	sample_rate,
	frames_per_buffer,
	paClipOff);
}
