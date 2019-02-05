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
    /* Initialise the PortAudio system and define audio parameters. */
    portaudio::AutoSystem auto_system;
    portaudio::System &system = portaudio::System::instance();
    AudioParameters audio_parameters(system, 20000, 256, 2, 2);

    /* Load database of buffers from root directory path. */
    std::string audio_file_dir = "./audio_files";
    Database<float> db;
    db.add_directory(audio_file_dir);

    if (argc < 2)
    {
	std::cout << "Usage: ./CATE <buffer index>\n";
	std::exit(0);
    }

    unsigned long db_index = std::atoi(argv[1]);

    if (db_index > (db.size()-1))
    {
	std::cout << "db index must be less than " << db.size() << ".\n";
	std::exit(0);
    }

    Synth<float> synth(db, db_index);

    /* Create a PortAudio stream for the Synth instance and start it on a new thread. */
    portaudio::MemFunCallbackStream<Synth<float>> stream(audio_parameters.get_stream(),
							 synth,
							 &Synth<float>::process);
    std::thread audio_thread(&portaudio::MemFunCallbackStream<Synth<float>>::start, &stream);
    audio_thread.detach();

    std::string filename = db[db_index].get_filename();
    std::cout << "Playing file: " << filename << '\n' << "Press Ctrl-C to exit.\n";
    while(true)
    {
    } 

    stream.stop();
    stream.close();
    system.terminate();

    return 0;
}
