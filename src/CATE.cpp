#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <thread>
#include <iostream>

#include "Wave.hpp"

#include "portaudio.h"
#include "sndfile.h"

namespace CATE
{
    int sampleRate = 44100;
    int framesPerBuffer = 256;

    class WavePlayerData
    {
    public:
	WavePlayerData(CATE::Wave wave)
	    : wave(wave)
	{
	}
	CATE::Wave wave;
	int position = 0;
	bool loop = true;
    };

    static int wavePlayer(const void *input, void *output,
			  unsigned long framesPerBuffer,
			  const PaStreamCallbackTimeInfo* timeInfo,
			  PaStreamCallbackFlags statusFlags, void *userData)
    {
	WavePlayerData *data = static_cast<WavePlayerData*>(userData);
	float *out = static_cast<float*>(output);
 
	for(unsigned long i = 0; i < framesPerBuffer; ++i)
	{
	    /* Looping position back to beginning when end is reached */
	    if (data->loop)
	    {
		if (data->position > data->wave.length)
		{
		    data->position = 0;
		}
	    }

	    /* Finished playing */
	    if (data->position > data->wave.length)
	    {
		return paComplete;
	    }

	    /* Left channel */
	    *out++ = data->wave.data[data->position];
	    data->position += 1;

	    /* Right channel */
	    *out++ = data->wave.data[data->position];
	    data->position += 1;
	}

	return paContinue;
    }
}

int main(int argc, char *argv[])
{
    Pa_Initialize();

    if (argc < 2)
    {
	std::cout << "Usage: CATE <audio file path>" << '\n';
	std::exit(0);
    }

    PaStream *stream = nullptr;
    PaStreamParameters outputParams;
    std::string audioFilePath = argv[1];
    CATE::Wave wave(audioFilePath);
    CATE::WavePlayerData wavePlayerData(wave);

    outputParams.device = Pa_GetDefaultOutputDevice();
    outputParams.channelCount = 2;      
    outputParams.sampleFormat = paFloat32;
    outputParams.suggestedLatency = Pa_GetDeviceInfo(outputParams.device)->defaultLowOutputLatency;
    outputParams.hostApiSpecificStreamInfo = nullptr;

    Pa_OpenStream(
	&stream,
	nullptr,
	&outputParams,
	CATE::sampleRate,
	CATE::framesPerBuffer,
	paClipOff, 
	CATE::wavePlayer,
	&wavePlayerData);

    std::thread audioThread(Pa_StartStream, stream);

    audioThread.detach();

    while (true)
    {
	std::cout << "Waiting for input...\r";
    }

    Pa_Terminate();

    return 0;
}
