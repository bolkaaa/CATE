#ifndef SYNTH_HPP
#define SYNTH_HPP

#include <cmath>

#include "AudioBuffer.hpp"
#include "Database.hpp"

#include "../lib/PortAudioCpp.hxx"

float foldback(float in, float threshold)
{
    if (in > threshold || in <= threshold)
    {
	in = fabs(fabs(std::fmod(in - threshold, threshold * 4)) - threshold * 2) - threshold;
    }
    return in;
}

template <class T>
class Synth
{
public:
    Synth();

    Synth(Database<T> db, unsigned int db_index)
	: db(db), db_index(db_index), buffer_pos(0)
    {
    }

    int process(const void *input, void *output,
		unsigned long frames_per_buffer,
		const PaStreamCallbackTimeInfo *time_info,
		PaStreamCallbackFlags status_flags)
    {
	T **out = static_cast<T**>(output);

	for (unsigned long i = 0; i < frames_per_buffer; ++i)
	{
	    /* Monophonic playback. (sample index increments once per sample as 
	       same sample is used for both channels. */
	    T source = db[db_index][buffer_pos];
	    T mix = foldback(source, 0.1);

	    out[0][i] = mix;
	    out[1][i] = mix;

	    buffer_pos += 1;

	    /* Exit loop when EOF reached. */
	    if (buffer_pos > db[db_index].size())
	    {
		return paComplete;
	    }
	}

	return paContinue;
    }

    AudioBuffer<T>& operator[](unsigned int i) { return db[i]; }

private:
    Database<T> db;
    unsigned int db_index;
    unsigned long buffer_pos;

};

#endif
