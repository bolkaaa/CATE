#ifndef SYNTH_HPP
#define SYNTH_HPP

#include <cmath>

#include "AudioBuffer.hpp"
#include "Database.hpp"

#include "portaudio.h"

template <class T>
class Synth
{
public:
    Synth();

    Synth(Database<T> db, std::string file)
	: db(db), file(file), buffer_pos(0)
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
	    T mix = db[file][buffer_pos];

	    /* Monophonic */
	    out[0][i] = mix;
	    out[1][i] = mix;

	    ++buffer_pos;

	    /* Exit loop when EOF reached. */
	    if (buffer_pos > db[file].size())
	    {
		return paComplete;
	    }
	}

	return paContinue;
    }

    AudioBuffer<T>& operator[](std::string file) { return db[file]; }

private:
    Database<T> db;
    std::string file;
    unsigned long buffer_pos;

};

#endif
