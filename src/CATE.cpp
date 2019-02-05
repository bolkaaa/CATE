#include <iostream>
#include <thread>
#include <vector>
#include <boost/filesystem.hpp>

#include "../lib/portaudio.h"
#include "../lib/PortAudioCpp.hxx"
#include "../lib/sndfile.h"
#include "../lib/sndfile.hh"

#include "Synth.hpp"
#include "AudioParameters.hpp"
#include "AudioBuffer.hpp"
#include "FileTree.hpp"
#include "Database.hpp"

int main(int argc, char *argv[])
{
    portaudio::AutoSystem auto_system;
    portaudio::System &system = portaudio::System::instance();
    AudioParameters audio_parameters(system, 20000, 256, 2, 2);

    /* Create std::vector of AudioBuffers and test Synth with one of the buffers. */
    std::string audio_file_dir = "./audio_files";
    Database<float> db;
    db.add_directory(audio_file_dir);

    Synth<float> synth(db[0]);

    /* Create a PortAudio stream for the Synth instance and start it on a new thread. */
    portaudio::MemFunCallbackStream<Synth<float>> stream(audio_parameters.get_stream(),
							 synth,
							 &Synth<float>::process);
    std::thread audio_thread(&portaudio::MemFunCallbackStream<Synth<float>>::start, &stream);
    audio_thread.detach();

    std::cout << "Press Ctrl-C to exit.\n";
    while(true)
    {
	std::cout << "OUTPUT: " << synth[synth.get_index()] << '\r';
    } // ...

    stream.stop();
    stream.close();
    system.terminate();

    return 0;
}
