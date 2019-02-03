#include "Synth.hpp"

Synth::Synth(AudioBuffer buffer)
    : buffer(buffer), index(0)
{
}

int Synth::process(const void *input, void *output,
		   unsigned long frames_per_buffer,
		   const PaStreamCallbackTimeInfo *time_info,
		   PaStreamCallbackFlags status_flags)
{
    float **out = static_cast<float**>(output);

    for (unsigned long i = 0; i < frames_per_buffer; ++i)
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

