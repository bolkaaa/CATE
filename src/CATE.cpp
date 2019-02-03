#include <iostream>
#include <thread>
#include <vector>

#include "../lib/portaudio.h"
#include "../lib/PortAudioCpp.hxx"
#include "../lib/sndfile.h"
#include "../lib/sndfile.hh"

#include "AudioParameters.hpp"
#include "AudioBuffer.hpp"
#include "Synth.hpp"

int main(int argc, char *argv[])
{
    portaudio::AutoSystem auto_system;
    portaudio::System &system = portaudio::System::instance();
    AudioParameters audio_parameters(system);

    /* Load an audio file into an AudioBuffer container and pass it to the 
       Synth object. */
    std::string audio_file_path = argv[1];
    AudioBuffer buffer(audio_file_path);
    Synth synth(buffer);

    /* Create a PortAudio stream for the Synth instance and start it on a new thread. */
    portaudio::MemFunCallbackStream<Synth> stream(audio_parameters.get_stream(),
						  synth,
						  &Synth::process);
    std::thread audio_thread(&portaudio::MemFunCallbackStream<Synth>::start, &stream);
    audio_thread.detach();

    while (1)
    {

    }

    /* Clean up stream and terminate PortAudio */
    stream.stop();
    stream.close();
    system.terminate();

    return 0;
}
