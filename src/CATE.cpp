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

std::string choose_file(int argc, char *argv[], Database<float> &db)
/* Basic command line arguments for choosing a file, for testing playback. */
{
    if (argc != 2)
    {
	std::string usage = "Usage: ./CATE <audio_file_path>";
	throw std::invalid_argument(usage);
    }

    std::string file = argv[1];

    bool exists = db.exists(file);

    if (!exists)
    {
	throw std::invalid_argument("File not found.");
    }

    return file;
}
	
int main(int argc, char *argv[])
{
    bool test = false;

    if (test)
    {
	// ...

	return 0;
    }

    /* Initialise the PortAudio system and define audio parameters. */
    portaudio::AutoSystem auto_system;
    portaudio::System &system = portaudio::System::instance();
    uint32_t sample_rate = 44100;
    uint16_t frames_per_buffer = 256;
    uint8_t input_channels = 2;
    uint8_t output_channels = 2;
    AudioParameters audio_parameters(system, sample_rate, frames_per_buffer,
				     input_channels, output_channels);

    /* Load database of buffers from root directory path. */
    std::string audio_file_dir = "./audio_files";
    Database<float> db;
    db.add_directory(audio_file_dir);
    db.convert_sample_rates(sample_rate);

    /* Choose file from database to test audio processing with. */
    std::string filename;
    try
    {
	filename = choose_file(argc, argv, db);
    }
    catch (std::invalid_argument &a)
    {
	std::cout << a.what() << '\n';
	return -1;
    }

    /* Testing segmentation of audio buffers from the audio file database.  */
    std::vector<AudioBuffer<float>> grains;
    uint16_t grain_size = 200;
    AudioBuffer<float> buffer = db.get_buffer(filename);
    buffer.segment(grains, grain_size, sample_rate);
    Synth<float> synth(grains, filename);

    /* Create a PortAudio stream for the Synth instance and start it on a new thread. */
    portaudio::MemFunCallbackStream<Synth<float>> stream(audio_parameters.get_stream(),
							 synth,
							 &Synth<float>::process);
    std::thread audio_thread(&portaudio::MemFunCallbackStream<Synth<float>>::start, &stream);
    audio_thread.detach();

    std::cout << "Playing file: " << filename << ".\nPress Ctrl-C to exit.\n";
    while(true)
    {
	// ...
    } 

    stream.stop();
    stream.close();
    system.terminate();

    return 0;
}

