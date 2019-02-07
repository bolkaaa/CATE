#include <iostream>
#include <thread>
#include <vector>
#include <boost/filesystem.hpp>

#include "portaudiocpp/PortAudioCpp.hxx"
#include "portaudio.h"
#include "sndfile.hh"
#include "samplerate.h"

#include "Synth.hpp"
#include "AudioParameters.hpp"
#include "AudioBuffer.hpp"
#include "FileTree.hpp"
#include "Database.hpp"

std::string test_args(int argc, char *argv[])
/* Basic command line arguments for testing audio files. */
{
    if (argc < 2)
    {
	std::cout << "Usage: ./CATE <file path>\n";
	std::exit(0);
    }

    std::string file_path = argv[1];

    return file_path;
}

int main(int argc, char *argv[])
{
    /* Initialise the PortAudio system and define audio parameters. */
    portaudio::AutoSystem auto_system;
    portaudio::System &system = portaudio::System::instance();
    unsigned int sample_rate = 44100;
    unsigned int frames_per_buffer = 256;
    unsigned int input_channels = 2;
    unsigned int output_channels = 2;
    AudioParameters audio_parameters(system, sample_rate, frames_per_buffer,
				     input_channels, output_channels);

    /* Load database of buffers from root directory path. */
    std::string audio_file_dir = "./audio_files";
    Database<float> db;
    db.add_directory(audio_file_dir);
    db.convert_sample_rates(sample_rate);

    /* Passing database to Synth object, currently just takes arbitrary filename. */
    std::string file = test_args(argc, argv);
    Synth<float> synth(db, file);

    /* Create a PortAudio stream for the Synth instance and start it on a new thread. */
    portaudio::MemFunCallbackStream<Synth<float>> stream(audio_parameters.get_stream(),
    							 synth,
    							 &Synth<float>::process);
    std::thread audio_thread(&portaudio::MemFunCallbackStream<Synth<float>>::start, &stream);
    audio_thread.detach();

    std::cout << "Playing file: " << file << ".\nPress Ctrl-C to exit.\n";
    while(true)
    {
    } 

    stream.stop();
    stream.close();
    system.terminate();

    return 0;
}

